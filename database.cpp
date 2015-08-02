#include "database.h"

#include <QDateTime>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QMap>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/qxmlquery.h>
#include <QtConcurrentRun>

#include <ctime>
#include <iostream>
#include <fstream>


#include "Error.h"

using namespace std;


void Database::save_cur_timestamp(QDomDocument &document, QDomElement &entry)
{
    time_t rawTime;
    struct tm * timeinfo;
    time(&rawTime);
    QDomElement timeE = document.createElement("timestamp");
    timeinfo = localtime(&rawTime);
    char *ascitime = asctime(timeinfo);
    ascitime[strlen(ascitime)-1] = '\0';
    QDomText timeT = document.createTextNode(ascitime);
    timeE.appendChild(timeT);
    entry.appendChild(timeE);
}

template <class T>
void save_field(QDomDocument &document, QDomElement &entry, const QString &field_name, T field_value)
{
    char buffer[64];  //for fake itoa

    QDomElement field = document.createElement(field_name);
    sprintf(buffer, "%d", field_value);
    QDomText startT = document.createTextNode(buffer);
    field.appendChild(startT);
    entry.appendChild(field);
}

QDomElement Database::create_entry(QDomDocument &doc, const QString &entry_name,
                                   const QString &root_name)
{
    QDomElement secCountE = doc.createElement(entry_name);
    doc.elementsByTagName(root_name).at(0).appendChild(secCountE);

    return secCountE;
}

SongInfo::SongInfo() : rating(2){
    songID = seconds = 0;
    albumName = songName = artistName = fileName = "";
}

void saveFileDB(QDomDocument &document, const string &filename){

    QFile outFile(QString(filename.c_str()));
    outFile.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream stream( &outFile );
    stream << document.toString();

    outFile.close();
    outFile.open(QIODevice::ReadOnly|QIODevice::Text);
    document.setContent(&outFile);
}

Database::Database(){
    char* root_path = "database";

    songs_db_changed = false;
    song_in_playlist_changed = false;

    if(!QDir(root_path).exists()){
        QDir().mkdir(root_path);
    }

    database = new XmlDatabase(root_path);
}

void Database::save_changed_dbs()
{
    if (songs_db_changed)
    {
        save_songs();
        songs_db_changed = false;
    }
    if (song_in_playlist_changed)
    {
        saveFileDB(database->songsInPlaylist, "database/songsInPlaylist.xml");
        song_in_playlist_changed = false;
    }
    if (playlist_changed)
    {
        saveFileDB(database->playlist, "database/playlist.xml");
        playlist_changed = false;
    }
}

void Database::save_sec_count_threaded(int ID, qint64 start, qint64 end)
{

    qDebug() << "Save Sec count:" << ID << " : " << start << "->" << end;

    // Stores the second count. Saves the song,
    // the second of the song started and stopped playing at
    // If whole song numbers would be 0 and song.length
    // stores the time this insert was recorded
    // end_sec - start_sec = number of seconds played since last insert

    QDomElement secCountE = create_entry(database->secCount, "secCount", "secRoot");

    save_field(database->secCount, secCountE, "ID", ID);

    save_field(database->secCount, secCountE, "startTime", start);
    save_field(database->secCount, secCountE, "endTime", end);

    save_cur_timestamp(database->secCount, secCountE);

    saveFileDB(database->secCount, "database/secCount.xml");

    save_changed_dbs();
}

void Database::save_sec_count(int ID, qint64 start, qint64 end)
{
    static QMutex sec_count_mutex;

    sec_count_mutex.lock();
    QtConcurrent::run(this, &Database::save_sec_count_threaded, ID, start, end);
    sec_count_mutex.unlock();
}

void Database::save_rating_count_threaded(int ID, int rating)
{
    QDomDocument &doc = database->ratingCount;
    const QString &xml_file = "database/ratingCount.xml";

    QDomElement entry = create_entry(doc, "ratingCount", "rateRoot");

    save_field(doc, entry, "ID", ID);

    save_field(doc, entry, "rating", rating);

    save_cur_timestamp(doc, entry);

    saveFileDB(doc, xml_file.toStdString());
}


void Database::save_rating_count(int ID, int rating)
{
    static QMutex rating_count_mutex;

    rating_count_mutex.lock();
    save_rating_count_threaded(ID, rating);
    rating_count_mutex.unlock();
}

QList<HotspotInfo>* Database::get_hotspot_info(){
    //returns a list of structs with:
    //song ID, list of hotspots(as qint64)

    QList<HotspotInfo> *ret = new QList<HotspotInfo>;
    HotspotInfo *curList = 0;
    int ID;
    QFile *spotFile = new QFile("database/hotspots.xml");
    spotFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader *spotReader = new QXmlStreamReader(spotFile);

    //create a map from the song ID to a list of its hotspots
    QMap<int, QList<qint64> > songIDToSpots;
    while(!spotReader->atEnd() && !spotReader->hasError()){
        //read next
        QXmlStreamReader::TokenType token = spotReader->readNext();
        if(token==QXmlStreamReader::StartDocument){
            continue;
        }
        if(token==QXmlStreamReader::StartElement){
            if(spotReader->name() == "hotspot"){
                continue;
            }
            if(spotReader->name() == "songID"){
                ID = spotReader->readElementText().toInt();
            }
            if(spotReader->name() == "spot"){
                songIDToSpots[ID].append(spotReader->readElementText().toInt());
            }
        }
    }
    spotFile->close();
    spotReader->clear();
    delete spotFile;
    delete spotReader;

    //iterate throuhg the map and put things into our struct
    QMap<int, QList<qint64> >::iterator it;
    for (it = songIDToSpots.begin(); it!=songIDToSpots.end(); it++) {
        curList = new HotspotInfo;
        curList->ID = it.key();
        curList->hotspots = it.value();
        ret->append(*curList);
    }
    return ret;
}

QList<PlaylistInfo>* Database::get_all_list_info(){
    //returns a list of structs with the following:
    // list of song IDs, list ID, list name

    QList<PlaylistInfo> *ret = new QList<PlaylistInfo>;
    QMap<int, int> listIDToPosition;
    PlaylistInfo *curList;
    int ID = 0;
    int i = 0;

    //go through the playlist file
    QFile *listFile = new QFile("database/playlist.xml");
    listFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader *listReader = new QXmlStreamReader(listFile);
    while(!listReader->atEnd() && !listReader->hasError()){
        //read next
        QXmlStreamReader::TokenType token = listReader->readNext();
        if(token==QXmlStreamReader::StartDocument){
            continue;
        }
        if(token==QXmlStreamReader::StartElement){
            if(listReader->name() == "playlist"){
                //we're in a new playlist
                //add a new struct to the list
                curList = 0;
                curList = new PlaylistInfo;
            }
            if(listReader->name() == "ID"){
                ID = listReader->readElementText().toInt();
                curList->ID = ID;
            }
            if(listReader->name() == "is_catalog"){
                curList->is_catalog = true;
            }
            if(listReader->name() == "name"){
                curList->name = listReader->readElementText();
                //map of playlist ID to main list position
                listIDToPosition[ID] = i++;
                //last thing, put the struct in the list
                ret->append(*curList);
            }
        }
    }
    listFile->close();
    listReader->clear();
    delete listFile;
    delete listReader;

    //go through the playlist relational file
    QFile *listRFile = new QFile("database/songsInPlaylist.xml");
    listRFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader *listRReader = new QXmlStreamReader(listRFile);
    int songID;
    int listPos;
    while(!listRReader->atEnd() && !listRReader->hasError()){
        //read next
        QXmlStreamReader::TokenType token = listRReader->readNext();
        if(token==QXmlStreamReader::StartDocument){
            continue;
        }
        if(token==QXmlStreamReader::StartElement){
            if(listRReader->name() == "songInPlaylist"){
                //we're in a new thing
                continue;
            }
            if(listRReader->name() == "songID"){
                songID = listRReader->readElementText().toInt();
            }
            if(listRReader->name() == "playlistID"){
                //look up the list position for this playlistID
                listPos = listIDToPosition[listRReader->readElementText().toInt()];
                //append the songID to ret[listPos].songIDs
                PlaylistInfo tempInfo;
                tempInfo = ret->takeAt(listPos);
                tempInfo.songIDs.append(songID);
                ret->insert(listPos, tempInfo);
            }
        }
    }
    listRFile->close();
    listRReader->clear();
    delete listRFile;
    delete listRReader;

    return ret;
}

#include "handle_data_value.h"

DataList *Database::get_sec_count_data()
{
     DataList *datalist = DataList::get_instance();

     Handle_Count *handler = new Add_Count_To_DataList(datalist);
     database->parse_sec_count_db(handler);

     return datalist;
}

QList<SongInfo>* Database::get_all_song_info(){
    //returns a list of structs of this info in this order:
    //ID, name, filename, created, seconds, album, artist

    QMap<int, SongInfo> songIDtoInfo = database->parse_song_db();

    //RATINGS!

    QMap<int, int> songIDToRating;
    Handle_Count *rating_handler = new Map_ID_to_Value(songIDToRating);

    database->parse_rating_db(rating_handler);

    delete rating_handler;

    //put all the counts from the map into the list
    QMap<int, int>::iterator id_rating_iter;
    for(id_rating_iter = songIDToRating.begin(); id_rating_iter != songIDToRating.end(); id_rating_iter++) {
        songIDtoInfo[id_rating_iter.key()].rating = id_rating_iter.value();
    }


    // Seconds
    QMap<int, int> songIDToSeconds;
    Handle_Count *sec_handler = new Map_ID_to_Total_Value(songIDToSeconds);

    database->parse_sec_count_db(sec_handler);

    delete sec_handler;

    //put all the counts from the map into the list
    QMap<int, int>::iterator id_sec_iter;
    for(id_sec_iter = songIDToSeconds.begin(); id_sec_iter!=songIDToSeconds.end(); id_sec_iter++) {
        songIDtoInfo[id_sec_iter.key()].seconds = id_sec_iter.value();
    }

    QList<SongInfo> *song_list = new QList<SongInfo>();
    QMap<int, SongInfo>::iterator song_info_iter;
    for(song_info_iter = songIDtoInfo.begin(); song_info_iter!=songIDtoInfo.end(); song_info_iter++) {
        song_list->append(song_info_iter.value());
    }

    return song_list;
}

void Database::update_song_name(int ID, const QString &new_name)
{
    qDebug() << "Database:" << ID << " : new_name : " << new_name;

    QDomNode song_root = database->song.elementsByTagName("songRoot").at(0);
    QDomNodeList song_list = song_root.childNodes();

    for (int i = 0; i < song_list.size() ; i++)
    {
        if (song_list.at(i).firstChildElement("ID").firstChild().toText().data() == QString::number(ID))
        {
            QDomElement nameE = database->song.createElement("name");
            QDomText nameT = database->song.createTextNode(new_name);
            nameE.appendChild(nameT);

            song_list.at(i).replaceChild(nameE, song_list.at(i).firstChildElement("name"));
            songs_db_changed = true;
            return;
        }
    }
}

void Database::add_song(int songID, const QString &filename,const QString &name, const QDateTime& created,
                        const QString &artistName, const QString &albumName, int albumID, int artistID)
{
    qDebug() << "At" << created << filename << "added to DB with ID =" << songID ;

    char temp[512];

    QXmlQuery query;
    QString output = "-1";
    sprintf(temp, "doc('database/song.xml')/songRoot/song[ID=%d]/ID/text()", songID);
    query.setQuery(temp);
    query.evaluateTo(&output);
    if(output.toDouble()!=songID){
        sprintf(temp, "doc('database/song.xml')/songRoot/song[name=%s]/name/text()", name.toStdString().c_str());
        query.setQuery(temp);
        query.evaluateTo(&output);
        if(output!=name){
            //Song

            QDomElement songE = database->song.createElement("song");
            database->song.elementsByTagName("songRoot").at(0).appendChild(songE);

            QDomElement IDE = database->song.createElement("ID");
            sprintf(temp, "%d", songID);
            QDomText IDT = database->song.createTextNode(temp);
            IDE.appendChild(IDT);
            songE.appendChild(IDE);

            QDomElement nameE = database->song.createElement("name");
            QDomText nameT = database->song.createTextNode(name);
            nameE.appendChild(nameT);
            songE.appendChild(nameE);

            QDomElement pathE = database->song.createElement("filepath");
            QDomText pathT = database->song.createTextNode(filename);
            pathE.appendChild(pathT);
            songE.appendChild(pathE);

            QDomElement createdE = database->song.createElement("created");
            QDomText createdT = database->song.createTextNode(created.toString());
            createdE.appendChild(createdT);
            songE.appendChild(createdE);

            songs_db_changed = true;
        }
    }

}

void Database::delete_song(int ID){

    QXmlQuery query;
    QString output;
    char filter[512];
    QFile write_file;
    QTextStream out(&write_file);

    //delete song from song.xml
    sprintf(filter, "doc('database/song.xml')/songRoot/song[ID!=%d]", ID);
    query.setQuery(filter);
    query.evaluateTo(&output);
    //output now has all <song> nodes
    write_file.setFileName("database/song.xml");
    write_file.open(QIODevice::ReadWrite);
    write_file.resize(0);
    out << "<songRoot>\n" << output << "\n</songRoot>";
    write_file.close();
    //reload the memory object
    write_file.open(QIODevice::ReadOnly|QIODevice::Text);
    database->song.setContent(&write_file);
    write_file.close();

}

void Database::save_songs()
{
    qDebug() << "Saving Songs";
    saveFileDB(database->song, "database/song.xml");
}

int Database::find_filename(const QString &name){
    // returns the ID of a song that has name as its name
    // -1 if name doesn't exit

    QXmlQuery query;
    QString output = "-1";
    QString input = "doc('database/song.xml')/songRoot/song[name=" + name + "]/ID/text()";
    query.setQuery(input);
    query.evaluateTo(&output);
    if(output.toDouble()!=-1){
        return output.toDouble();
    }
    return -1;
}

int Database::find_filepath(const QString &filepath){
    // returns the ID of a song that has filepath as its filepath
    // -1 if filepath doesn't exit

    QXmlQuery query;
    QString output = "-1";
    QString input = "doc('database/song.xml')/songRoot/song[filepath=" + filepath + "]/ID/text()";
    query.setQuery(input);
    query.evaluateTo(&output);
    if(output.toDouble()!=-1){
        return output.toDouble();
    }
    return -1;
}


void Database::add_hotspot(int songID, qint64 hotspot){

    char temp[256];
    QXmlQuery query;
    QString output = "-1";
    sprintf(temp, "doc('database/hotspots.xml')/hotspotRoot/hotspot[songID=%d and spot=%d]/songID/text()", songID, hotspot);
    query.setQuery(temp);
    query.evaluateTo(&output);
    if(output.toDouble()!=songID){
        QDomElement hotspotE = database->hotspots.createElement("hotspot");
        database->hotspots.elementsByTagName("hotspotRoot").at(0).appendChild(hotspotE);

        QDomElement songIDE = database->hotspots.createElement("songID");
        sprintf(temp, "%d", songID);
        QDomText songIDT = database->hotspots.createTextNode(temp);
        songIDE.appendChild(songIDT);
        hotspotE.appendChild(songIDE);

        QDomElement spotE = database->hotspots.createElement("spot");
        sprintf(temp, "%d", hotspot);
        QDomText spotT = database->hotspots.createTextNode(temp);
        spotE.appendChild(spotT);
        hotspotE.appendChild(spotE);

        saveFileDB(database->hotspots, "database/hotspots.xml");
    }
}

void Database::delete_hotspot(int songID, qint64 hotspot){

    QXmlQuery query;
    QString output;
    char filter[256];
    QFile write_file;
    QTextStream out(&write_file);

    //delete hotspot
    sprintf(filter, "doc('database/hotspots.xml')/hotspotRoot/hotspot[not(songID=%d and spot=%d)]", songID, hotspot);
    query.setQuery(filter);
    query.evaluateTo(&output);
    //output now has all <song> nodes
    write_file.setFileName("database/hotspots.xml");
    write_file.open(QIODevice::ReadWrite);
    write_file.resize(0);
    out << "<hotspotRoot>/n" << output << "</hotspotRoot>";
    write_file.close();
    //reload the memory object
    write_file.open(QIODevice::ReadOnly|QIODevice::Text);
    database->hotspots.setContent(&write_file);
    write_file.close();
}

void Database::new_playlist(const QString &name, int ID, bool is_catalog) {
    qDebug() << "Database: Adding new Playlist:" << name;

    char temp[256];
    QXmlQuery query;
    QString output = "-1";
    sprintf(temp, "doc('database/playlist.xml')/playlistRoot/playlist[ID=%d]/ID/text()", ID);
    query.setQuery(temp);
    query.evaluateTo(&output);

    if(output.toDouble()!=ID){
        QDomElement playlistE = database->playlist.createElement("playlist");
        database->playlist.elementsByTagName("playlistRoot").at(0).appendChild(playlistE);

        QDomElement IDE = database->playlist.createElement("ID");
        sprintf(temp, "%d", ID);
        QDomText IDT = database->playlist.createTextNode(temp);
        IDE.appendChild(IDT);
        playlistE.appendChild(IDE);

        if (is_catalog)
        {
            QDomElement is_cat_E = database->playlist.createElement("is_catalog");
            QDomText is_cat_T = database->playlist.createTextNode("true");
            is_cat_E.appendChild(is_cat_T);
            playlistE.appendChild(is_cat_E);
        }

        QDomElement nameE = database->playlist.createElement("name");
        QDomText nameT = database->playlist.createTextNode(name);
        nameE.appendChild(nameT);
        playlistE.appendChild(nameE);

        playlist_changed = true;
    }
}

void Database::edit_playlist_name(int listID, QString newName){
    //okay, we're going to do this real classy-like. You ready?
    //we're going to delete the playlist entry and put it back in with a different name.
    //I know, I'm impressed too.

    //delete the playlist entry from playlist.xml
    QXmlQuery query;
    QString output;
    char filter[256];
    QFile write_file;
    QTextStream out(&write_file);

    sprintf(filter, "doc('database/playlist.xml')/playlistRoot/playlist[not(ID=%d)]", listID);
    query.setQuery(filter);
    query.evaluateTo(&output);
    //output now has all <playlist> nodes
    write_file.setFileName("database/playlist.xml");
    write_file.open(QIODevice::ReadWrite);
    write_file.resize(0);
    out << "<playlistRoot>/n" << output << "</playlistRoot>";
    write_file.close();
    //reload the memory object
    write_file.open(QIODevice::ReadOnly|QIODevice::Text);
    database->playlist.setContent(&write_file);
    write_file.close();

    //insert into playlist.xml with ID and name
    char temp[256];
    QDomElement playlistE = database->playlist.createElement("playlist");
    database->playlist.elementsByTagName("playlistRoot").at(0).appendChild(playlistE);

    QDomElement listIDE = database->playlist.createElement("ID");
    sprintf(temp, "%d", listID);
    QDomText listIDT = database->playlist.createTextNode(temp);
    listIDE.appendChild(listIDT);
    playlistE.appendChild(listIDE);

    QDomElement nameE = database->playlist.createElement("name");
    sprintf(temp, "%d", newName.toStdString().c_str());
    QDomText nameT = database->playlist.createTextNode(temp);
    nameE.appendChild(nameT);
    playlistE.appendChild(nameE);

    playlist_changed = true;
}

void Database::add_to_playlist(int songID, int listID){
    char temp[256];
    QXmlQuery query;
    QString output = "-1";
    sprintf(temp, "doc('database/songsInPlaylist.xml')/SIPRoot/songInPlaylist[songID=%d and playlistID=%d]/songID/text()", songID, listID);
    query.setQuery(temp);
    query.evaluateTo(&output);
    if(output.toInt()!=songID){
        //relation not already there
        QDomElement SIPE = database->songsInPlaylist.createElement("songInPlaylist");
        database->songsInPlaylist.elementsByTagName("SIPRoot").at(0).appendChild(SIPE);

        QDomElement SIDE = database->songsInPlaylist.createElement("songID");
        sprintf(temp, "%d", songID);
        QDomText SIDT = database->songsInPlaylist.createTextNode(temp);
        SIDE.appendChild(SIDT);
        SIPE.appendChild(SIDE);

        QDomElement LIDE = database->songsInPlaylist.createElement("playlistID");
        sprintf(temp, "%d", listID);
        QDomText LIDT = database->songsInPlaylist.createTextNode(temp);
        LIDE.appendChild(LIDT);
        SIPE.appendChild(LIDE);

        song_in_playlist_changed = true;
    }

    qDebug() << "Database: Added" << songID << "to" << listID;
}

void Database::delete_playlist(int listID){
    QXmlQuery query;
    QString output;
    char filter[256];
    QFile write_file;
    QTextStream out(&write_file);

    qDebug() << "DELETING PLAYLIST";

    sprintf(filter, "doc('database/playlist.xml')/playlistRoot/playlist[not(ID=%d)]", listID);
    query.setQuery(filter);
    query.evaluateTo(&output);
    //output now has all <playlist> nodes
    write_file.setFileName("database/playlist.xml");
    write_file.open(QIODevice::ReadWrite);
    write_file.resize(0);
    out << "<playlistRoot>/n" << output << "</playlistRoot>";
    write_file.close();
    //reload the memory object
    write_file.open(QIODevice::ReadOnly|QIODevice::Text);
    database->playlist.setContent(&write_file);
    write_file.close();

    //delete all the entries in songsInPlaylist.xml involving that playlist
    sprintf(filter, "doc('database/songsInPlaylist.xml')/SIPRoot/songInPlaylist[playlistID!=%d]", listID);
    query.setQuery(filter);
    query.evaluateTo(&output);
    //output now has all <playlist> nodes
    write_file.setFileName("database/songsInPlaylist.xml");
    write_file.open(QIODevice::ReadWrite);
    write_file.resize(0);
    out << "<SIPRoot>/n" << output << "</SIPRoot>";
    write_file.close();
    //reload the memory object
    write_file.open(QIODevice::ReadOnly|QIODevice::Text);
    database->songsInPlaylist.setContent(&write_file);
    write_file.close();
}

void Database::delete_from_playlist(int songID, int listID){

    QXmlQuery query;
    QString output;
    char filter[256];
    QFile write_file;
    QTextStream out(&write_file);

    //delete song from songsInPlaylist
    sprintf(filter, "doc('database/songsInPlaylist.xml')/SIPRoot/songInPlaylist[not(songID=%d and playlistID=%d)]", songID, listID);
    query.setQuery(filter);
    query.evaluateTo(&output);
    //output now has all <songInPlaylist> nodes
    write_file.setFileName("database/songsInPlaylist.xml");
    write_file.open(QIODevice::ReadWrite);
    write_file.resize(0);
    out << "<SIPRoot>\n" << output << "\n</SIPRoot>";
    write_file.close();
    //reload the memory object
    write_file.open(QIODevice::ReadOnly|QIODevice::Text);
    database->songsInPlaylist.setContent(&write_file);
    write_file.close();


}
