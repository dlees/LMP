#include "database.h"

#include <QDateTime>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QMap>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/qxmlquery.h>
#include <ctime>
#include <iostream>
#include <fstream>

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

void Database::saveFile(QDomDocument &document, const QString &filename){

    QFile outFile(filename);
    outFile.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream stream( &outFile );
    stream << document.toString();

    outFile.close();
    outFile.open(QIODevice::ReadOnly|QIODevice::Text);
    document.setContent(&outFile);
}

void Database::load_file(const string &filename, QDomDocument &document, const QString &root_name)
{
    ofstream temp;

    ifstream fin(filename.data());
    if(!fin){
        temp.open(filename.data());
        temp.close();
    }
    //load file
    QFile qfile(filename.data());
    document.setContent(&qfile);
    if(!fin){
        QDomElement root = document.createElement(root_name);
        document.appendChild(root);
        saveFile(document, filename.data());
    }
    qfile.close();
}

Database::Database(){
    //load or create all files/tables

    ofstream temp;
    if(!QDir("database").exists()){
        QDir().mkdir("database");
    }

    //Song
    ifstream songStream("database/song.xml");
    if(!songStream){
        temp.open("database/song.xml");
        temp.close();
    }
    //load file
    QFile songFile("database/song.xml");
    song.setContent(&songFile);
    if(!songStream){
        QDomElement root = song.createElement("songRoot");
        song.appendChild(root);
        saveFile(song, "database/song.xml");
    }
    songFile.close();

    //Playlist
    ifstream playlistStream("database/playlist.xml");
    if(!playlistStream){
        //create file
        temp.open("database/playlist.xml");
        temp.close();
    }
    //load file
    QFile playlistFile("database/playlist.xml");
    playlist.setContent(&playlistFile);
    if(!playlistStream){
        QDomElement root = playlist.createElement("playlistRoot");
        playlist.appendChild(root);
        saveFile(playlist, "database/playlist.xml");
    }
    playlistFile.close();

    //Album
    ifstream albumStream("database/album.xml");
    if(!albumStream){
        //create file
        temp.open("database/album.xml");
        temp.close();
    }
    //load file
    QFile albumFile("database/album.xml");
    album.setContent(&albumFile);
    if(!albumStream){
        QDomElement root = album.createElement("albumRoot");
        album.appendChild(root);
        saveFile(album, "database/album.xml");
    }
    albumFile.close();

    //Artist
    ifstream artistStream("database/artist.xml");
    if(!artistStream){
        temp.open("database/artist.xml");
        temp.close();
    }
    //load file
    QFile artistFile("database/artist.xml");
    artist.setContent(&artistFile);
    if(!artistStream){
        QDomElement root = artist.createElement("artistRoot");
        artist.appendChild(root);
        saveFile(artist, "database/artist.xml");
    }
    artistFile.close();

    //SecCount
    load_file("database/secCount.xml", secCount, "secRoot");

    //RatingCount
    load_file("database/ratingCount.xml", ratingCount, "rateRoot");

    //SongsInPlaylist
    ifstream SIPStream("database/songsInPlaylist.xml");
    if(!SIPStream){
        temp.open("database/songsInPlaylist.xml");
        temp.close();
    }
    //load file
    QFile SIPFile("database/songsInPlaylist.xml");
    songsInPlaylist.setContent(&SIPFile);
    if(!SIPStream){
        QDomElement root = songsInPlaylist.createElement("SIPRoot");
        songsInPlaylist.appendChild(root);
        saveFile(songsInPlaylist, "database/songsInPlaylist.xml");
    }
    SIPFile.close();

    //SongsOnAlbum
    ifstream SOAStream("database/songsOnAlbum.xml");
    if(!SOAStream){
        temp.open("database/songsOnAlbum.xml");
        temp.close();
    }
    //load file
    QFile SOAFile("database/songsOnAlbum.xml");
    songsOnAlbum.setContent(&SOAFile);
    if(!SOAStream){
        QDomElement root = songsOnAlbum.createElement("SOARoot");
        songsOnAlbum.appendChild(root);
        saveFile(songsOnAlbum, "database/songsOnAlbum.xml");
    }
    SOAFile.close();

    //AlbumsByArtist
    ifstream ABAStream("database/albumsByArtist.xml");
    if(!ABAStream){
        temp.open("database/albumsByArtist.xml");
        temp.close();
    }
    //load file
    QFile ABAFile("database/albumsByArtist.xml");
    albumsByArtist.setContent(&ABAFile);
    if(!ABAStream){
        QDomElement root = albumsByArtist.createElement("ABARoot");
        albumsByArtist.appendChild(root);
        saveFile(albumsByArtist, "database/albumsByArtist.xml");
    }
    ABAFile.close();

    //hotspots
    ifstream HStream("database/hotspots.xml");
    if(!HStream){
        temp.open("database/hotspots.xml");
        temp.close();
    }
    //load file
    QFile HFile("database/hotspots.xml");
    hotspots.setContent(&HFile);
    if(!ABAStream){
        QDomElement root = hotspots.createElement("hotspotRoot");
        hotspots.appendChild(root);
        saveFile(hotspots, "database/hotspots.xml");
    }
    HFile.close();

    //any other initialization goes here
}

void Database::save_sec_count(int ID, qint64 start, qint64 end)
{
    qDebug() << ID << " : " << start << "->" << end;

    // Stores the second count. Saves the song,
    // the second of the song started and stopped playing at
    // If whole song numbers would be 0 and song.length
    // stores the time this insert was recorded
    // end_sec - start_sec = number of seconds played since last insert

    QDomElement secCountE = create_entry(secCount, "secCount", "secRoot");

    save_field(secCount, secCountE, "ID", ID);

    save_field(secCount, secCountE, "startTime", start);
    save_field(secCount, secCountE, "endTime", end);

    save_cur_timestamp(secCount, secCountE);

    saveFile(secCount, "database/secCount.xml");
}

void Database::save_rating_count(int ID, int rating)
{
    QDomDocument &doc = ratingCount;
    const QString &xml_file = "database/ratingCount.xml";

    QDomElement entry = create_entry(doc, "ratingCount", "rateRoot");

    save_field(doc, entry, "ID", ID);

    save_field(doc, entry, "rating", rating);

    save_cur_timestamp(doc, entry);

    saveFile(doc, xml_file);
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

QList<SongInfo>* Database::get_all_song_info(){
    //returns a list of structs of this info in this order:
    //ID, name, filename, created, seconds, album, artist

    QList<SongInfo> *song_list = new QList< SongInfo >;
    int i = 0;
    SongInfo *curSong = 0;
    QMap<int, int> songIDToList;
    QString temp;

    QFile *songFile = new QFile("database/song.xml");
    songFile->open(QIODevice::ReadOnly | QIODevice::Text);

    QXmlStreamReader *songReader = new QXmlStreamReader(songFile);
    while(!songReader->atEnd() && !songReader->hasError()){
        //read next
        QXmlStreamReader::TokenType token = songReader->readNext();
        if(token==QXmlStreamReader::StartDocument){
            continue;
        }
        if(token==QXmlStreamReader::StartElement){
            if(songReader->name() == "song"){
                //we're in a new song
                //add a new list to our list list
                if (curSong)
                    delete curSong;
                curSong = new SongInfo;
            }
            if(songReader->name() == "ID"){
                temp = songReader->readElementText();
                curSong->songID = temp.toInt();
                songIDToList[temp.toInt()] = i++;
            }
            if(songReader->name() == "name"){
                curSong->songName = songReader->readElementText();
            }
            if(songReader->name() == "filepath"){
                curSong->fileName = songReader->readElementText();
            }
            if(songReader->name() == "created"){
                QDateTime temp;
                curSong->created = temp.fromString(songReader->readElementText());
                //last one
                song_list->append(*curSong);
            }
        }
    }
    songFile->close();
    songReader->clear();
    delete songFile;
    delete songReader;

    //RATINGS!

    QFile *ratingFile = new QFile("database/ratingCount.xml");
    ratingFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader *ratingReader = new QXmlStreamReader(ratingFile);

    QMap<int, int> songIDToRating;

    int ID = 0;

    while(!ratingReader->atEnd() && !ratingReader->hasError()){
        //read next
        QXmlStreamReader::TokenType token = ratingReader->readNext();
        if(token==QXmlStreamReader::StartDocument){
            continue;
        }
        if(token==QXmlStreamReader::StartElement){
            if(ratingReader->name() == "ratingCount"){
                //in a new count
                continue;
            }
            if(ratingReader->name() == "ID"){
                temp = ratingReader->readElementText();
                ID = temp.toInt();
            }
            if(ratingReader->name() == "rating"){
                temp = ratingReader->readElementText();
                songIDToRating[ID] = (temp.toInt());
            }
        }
    }
    //put all the counts from the map into the list
    QMap<int, int>::iterator id_rating_iter;
    int listPos = 0;
    //QString* stringPtr;
    for(id_rating_iter = songIDToRating.begin();
        id_rating_iter != songIDToRating.end();
        id_rating_iter++) {
        listPos = songIDToList[id_rating_iter.key()];
        SongInfo tempInfo;
        tempInfo = song_list->takeAt(listPos);
        tempInfo.rating = id_rating_iter.value();


        song_list->insert(listPos, tempInfo);

    }

    ratingFile->close();
    ratingReader->clear();
    delete ratingFile;
    delete ratingReader;

// Seconds
    QFile *secFile = new QFile("database/secCount.xml");
    secFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader *secReader = new QXmlStreamReader(secFile);

    QMap<int, int> songIDToSeconds;
    int startTime = 0;
    ID = 0;
    //iterate through songIDToList, put all the IDs and 0 into the songIDToSeconds map
    QMap<int, int>::iterator iter;
    iter = songIDToList.begin();
    for (iter = songIDToList.begin(); iter!=songIDToList.end(); iter++) {
        songIDToSeconds[iter.key()] = 0;
    }

    iter = songIDToList.begin();
    while(!secReader->atEnd() && !secReader->hasError()){
        //read next
        QXmlStreamReader::TokenType token = secReader->readNext();
        if(token==QXmlStreamReader::StartDocument){
            continue;
        }
        if(token==QXmlStreamReader::StartElement){
            if(secReader->name() == "secCount"){
                //in a new count
                continue;
            }
            if(secReader->name() == "ID"){
                temp = secReader->readElementText();
                ID = temp.toInt();
            }
            if(secReader->name() == "startTime"){
                temp = secReader->readElementText();
                startTime = temp.toInt();
            }
            if(secReader->name() == "endTime"){
                temp = secReader->readElementText();
                int ti;
                ti = songIDToSeconds[ID];
                songIDToSeconds[ID] = ti + (temp.toInt() - startTime);
            }
        }
    }
    //put all the counts from the map into the list
    QMap<int, int>::iterator id_sec_iter;
    listPos = 0;
    //QString* stringPtr;
    for(id_sec_iter = songIDToSeconds.begin(); id_sec_iter!=songIDToSeconds.end(); id_sec_iter++) {
        listPos = songIDToList[id_sec_iter.key()];
        SongInfo tempInfo;
        tempInfo = song_list->takeAt(listPos);
        tempInfo.seconds = id_sec_iter.value();
        song_list->insert(listPos, tempInfo);
    }

/*
    //albums
    //need a map called albumIDToSong
    //also need to insert album name

    QFile *albumFile = new QFile("database/album.xml");
    albumFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader *albumReader = new QXmlStreamReader(albumFile);

    QMap<int, QString> albumIDToName;
    QString name;

    while(!albumReader->atEnd() && !albumReader->hasError()){
        //read next
        QXmlStreamReader::TokenType token = albumReader->readNext();
        if(token==QXmlStreamReader::StartDocument){
            continue;
        }
        if(token==QXmlStreamReader::StartElement){
            if(albumReader->name() == "album"){
                //in a new count
                continue;
            }
            if(albumReader->name() == "name"){
                name = albumReader->readElementText();
            }
            if(albumReader->name() == "ID"){
                temp = albumReader->readElementText();
                albumIDToName[temp.toInt()] = name;
            }
        }
    }

    albumFile->close();
    albumReader->clear();
    delete albumFile;
    delete albumReader;

    QFile *albFile = new QFile("database/songsOnAlbum.xml");
    albFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader *albReader = new QXmlStreamReader(albFile);

    QMap<int, int> albumIDToSong;

    while(!albReader->atEnd() && !albReader->hasError()){
        //read next
        QXmlStreamReader::TokenType token = albReader->readNext();
        if(token==QXmlStreamReader::StartDocument){
            continue;
        }
        if(token==QXmlStreamReader::StartElement){
            if(albReader->name() == "songOnAlbum"){
                //in a new thing
                continue;
            }
            if(albReader->name() == "songID"){
                temp = albReader->readElementText();
                ID = temp.toInt();
            }
            if(albReader->name() == "albumID"){
                temp = albReader->readElementText();
                albumIDToSong[temp.toInt()] = ID;

                //put the album name in the list
                name = albumIDToName[temp.toInt()];
                listPos = songIDToList[ID];

                SongInfo tempInfo;
                tempInfo = song_list->takeAt(listPos);
                tempInfo.albumName = name;
                song_list->insert(listPos, tempInfo);
            }
        }
    }
    albFile->close();
    albReader->clear();
    delete albFile;
    delete albReader;

    //now for the artists
    //we still have that map of ids to positions, which will come in handy
    //we might as well just create a map of artists to ids first
    //I really hope this doesn't end up taking like 5 minutes

    QFile *artFile = new QFile("database/artist.xml");
    artFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader *artReader = new QXmlStreamReader(artFile);

    QMap<int, QString> artIDToName;

    while(!artReader->atEnd() && !artReader->hasError()){
        //read next
        QXmlStreamReader::TokenType token = artReader->readNext();
        if(token==QXmlStreamReader::StartDocument){
            continue;
        }
        if(token==QXmlStreamReader::StartElement){
            if(artReader->name() == "artist"){
                //in a new count
                continue;
            }
            if(artReader->name() == "name"){
                name = artReader->readElementText();
            }
            if(artReader->name() == "ID"){
                temp = artReader->readElementText();
                artIDToName[temp.toInt()] = name;
            }
        }
    }

    artFile->close();
    artReader->clear();
    delete artFile;
    delete artReader;

    //now get the album/artist relationship, and add to the list
    //this function is going to be way too long, but c'est la vie.
    QFile *aRelFile = new QFile("database/albumsByArtist.xml");
    aRelFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader *aRelReader = new QXmlStreamReader(aRelFile);

    int albumID;

    while(!aRelReader->atEnd() && !aRelReader->hasError()){
        //read next
        QXmlStreamReader::TokenType token = aRelReader->readNext();
        if(token==QXmlStreamReader::StartDocument){
            continue;
        }
        if(token==QXmlStreamReader::StartElement){
            if(aRelReader->name() == "albumByArtist"){
                //in a new ABA
                continue;
            }
            if(aRelReader->name() == "albumID"){
                temp = aRelReader->readElementText();
                albumID = temp.toInt();
            }
            if(aRelReader->name() == "artistID"){
                temp = aRelReader->readElementText();
                //put the artist name in the list
                ID = albumIDToSong[albumID];
                name = artIDToName[temp.toInt()];
                listPos = songIDToList[ID];

                SongInfo tempInfo;
                tempInfo = song_list->takeAt(listPos);
                tempInfo.artistName = name;
                song_list->insert(listPos, tempInfo);

                //ret[listPos].artistName = name;

                //have to take it out to change it
                //QList<QString> tempList;
                //tempList = ret->takeAt(listPos);
                //tempList.append(name);
                //ret->insert(listPos, tempList);
            }
        }
    }
    aRelFile->close();
    aRelReader->clear();
    delete aRelFile;
    delete aRelReader;
*/
    return song_list;
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

            QDomElement songE = song.createElement("song");
            song.elementsByTagName("songRoot").at(0).appendChild(songE);

            QDomElement IDE = song.createElement("ID");
            sprintf(temp, "%d", songID);
            QDomText IDT = song.createTextNode(temp);
            IDE.appendChild(IDT);
            songE.appendChild(IDE);

            QDomElement nameE = song.createElement("name");
            QDomText nameT = song.createTextNode(name);
            nameE.appendChild(nameT);
            songE.appendChild(nameE);

            QDomElement pathE = song.createElement("filepath");
            QDomText pathT = song.createTextNode(filename);
            pathE.appendChild(pathT);
            songE.appendChild(pathE);

            QDomElement createdE = song.createElement("created");
            QDomText createdT = song.createTextNode(created.toString());
            createdE.appendChild(createdT);
            songE.appendChild(createdE);

            saveFile(song, "database/song.xml");

            //songOnAlbum
            QDomElement onAlbumE = songsOnAlbum.createElement("songOnAlbum");
            songsOnAlbum.elementsByTagName("SOARoot").at(0).appendChild(onAlbumE);

            QDomElement songIDE = songsOnAlbum.createElement("songID");
            sprintf(temp, "%d", songID);
            QDomText songIDT = songsOnAlbum.createTextNode(temp);
            songIDE.appendChild(songIDT);
            onAlbumE.appendChild(songIDE);

            QDomElement albumIDE = songsOnAlbum.createElement("albumID");
            sprintf(temp, "%d", albumID);
            QDomText albumIDT = songsOnAlbum.createTextNode(temp);
            albumIDE.appendChild(albumIDT);
            onAlbumE.appendChild(albumIDE);

            saveFile(songsOnAlbum, "database/songsOnAlbum.xml");
        }
    }

    sprintf(temp, "doc('database/album.xml')/albumRoot/album[ID=%d]/ID/text()", albumID);
    query.setQuery(temp);
    output = "-1";
    query.evaluateTo(&output);
    if(output.toDouble()!=albumID){
        //album
        QDomElement albumE = album.createElement("album");
        album.elementsByTagName("albumRoot").at(0).appendChild(albumE);

        QDomElement nameE = album.createElement("name");
        QDomText nameT = album.createTextNode(albumName);
        nameE.appendChild(nameT);
        albumE.appendChild(nameE);

        QDomElement IDE = album.createElement("ID");
        sprintf(temp, "%d", albumID);
        QDomText IDT = album.createTextNode(temp);
        IDE.appendChild(IDT);
        albumE.appendChild(IDE);

        saveFile(album, "database/album.xml");

        //AlbumByArtist
        QDomElement byArtistE = albumsByArtist.createElement("albumByArtist");
        albumsByArtist.elementsByTagName("ABARoot").at(0).appendChild(byArtistE);

        QDomElement albumIDE = albumsByArtist.createElement("albumID");
        sprintf(temp, "%d", albumID);
        QDomText albumIDT = albumsByArtist.createTextNode(temp);
        albumIDE.appendChild(albumIDT);
        byArtistE.appendChild(albumIDE);

        QDomElement artistIDE = albumsByArtist.createElement("artistID");
        sprintf(temp, "%d", artistID);
        QDomText artistIDT = albumsByArtist.createTextNode(temp);
        artistIDE.appendChild(artistIDT);
        byArtistE.appendChild(artistIDE);

        saveFile(albumsByArtist, "database/albumsByArtist.xml");
    }
    sprintf(temp, "doc('database/artist.xml')/artistRoot/artist[ID=%d]/ID/text()", artistID);
    query.setQuery(temp);
    output = "-1";
    query.evaluateTo(&output);
    if(output.toDouble()!=artistID){
        //artist
        QDomElement artistE = artist.createElement("artist");
        artist.elementsByTagName("artistRoot").at(0).appendChild(artistE);

        QDomElement nameE = artist.createElement("name");
        QDomText nameT = artist.createTextNode(artistName);
        nameE.appendChild(nameT);
        artistE.appendChild(nameE);

        QDomElement IDE = artist.createElement("ID");
        sprintf(temp, "%d", artistID);
        QDomText IDT = artist.createTextNode(temp);
        IDE.appendChild(IDT);
        artistE.appendChild(IDE);

        saveFile(artist, "database/artist.xml");
    }
}

void Database::delete_song(int ID){

    QXmlQuery query;
    QString output;
    char filter[512];
    QFile write_file;
    QTextStream out(&write_file);
    int albumID;
    int artistID;

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
    song.setContent(&write_file);
    write_file.close();

    //make sure we know the album
    sprintf(filter, "doc('database/songsOnAlbum.xml')/SOARoot/songOnAlbum[songID=%d]/albumID/text()", ID);
    query.setQuery(filter);
    query.evaluateTo(&output);
    albumID = output.toInt();

    //delete line from songsOnAlbum
    sprintf(filter, "doc('database/songsOnAlbum.xml')/SOARoot/songOnAlbum[songID!=%d]", ID);
    query.setQuery(filter);
    query.evaluateTo(&output);
    //output now has all <songOnAlbum> nodes
    write_file.setFileName("database/songsOnAlbum.xml");
    write_file.open(QIODevice::ReadWrite);
    write_file.resize(0);
    out << "<SOARoot>\n" << output << "</SOARoot>";
    write_file.close();
    //reload the memory object
    write_file.open(QIODevice::ReadOnly|QIODevice::Text);
    songsOnAlbum.setContent(&write_file);
    write_file.close();

    //see if there's anything left in the album
    sprintf(filter, "doc('database/songsOnAlbum.xml')/SOARoot/songOnAlbum[albumID=%d]/albumID/text()", albumID);
    query.setQuery(filter);
    query.evaluateTo(&output);
    //if it was the only song on an album
    if(output.toInt()!=albumID){
        //get the artist ID
        sprintf(filter, "doc('database/albumsByArtist.xml')/ABARoot/albumByArtist[albumID=%d]/artistID/text()", albumID);
        query.setQuery(filter);
        query.evaluateTo(&output);
        artistID = output.toInt();

        //delete the album
        sprintf(filter, "doc('database/album.xml')/albumRoot/album[ID!=%d]", albumID);
        query.setQuery(filter);
        query.evaluateTo(&output);
        //output now has all <songOnAlbum> nodes
        write_file.setFileName("database/album.xml");
        write_file.open(QIODevice::ReadWrite);
        write_file.resize(0);
        out << "<albumRoot>\n" << output << "</albumRoot>";
        write_file.close();
        //reload the memory object
        write_file.open(QIODevice::ReadOnly|QIODevice::Text);
        album.setContent(&write_file);
        write_file.close();
    }

    //see if there's anything left by the artist
    sprintf(filter, "doc('database/albumsByArtist.xml')/ABARoot/albumByArtist[artistID=%d]/artistID/text()", artistID);
    query.setQuery(filter);
    query.evaluateTo(&output);
    //if that album was the only album by an artist
    if(output.toInt()!=artistID){
        //delete the artist
        sprintf(filter, "doc('database/artist.xml')/artistRoot/artist[ID!=%d]", artistID);
        query.setQuery(filter);
        query.evaluateTo(&output);
        //output now has all <songOnAlbum> nodes
        write_file.setFileName("database/artist.xml");
        write_file.open(QIODevice::ReadWrite);
        write_file.resize(0);
        out << "<artistRoot>\n" << output << "</artistRoot>";
        write_file.close();
        //reload the memory object
        write_file.open(QIODevice::ReadOnly|QIODevice::Text);
        artist.setContent(&write_file);
        write_file.close();
    }
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

QList<int> Database::find(const QString &str){
    // returns all the id's of anything in the database
    // that has str in it (Songs, Artists, Albums, Playlists)

    QList<int> IDs;
    QXmlQuery query;
    QString input;
    QString output;

    //check songs
    output = "-1";
    input = "doc('database/song.xml')/songRoot/song[name=" + str + "]/ID/text()";
    query.setQuery(input);
    query.evaluateTo(&output);
    if(output.toDouble()!=-1){
        IDs.append(output.toDouble());
    }

    //check artists
    output = "-1";
    input = "doc('database/artist.xml')/artistRoot/artist[name=" + str + "]/ID/text()";
    query.setQuery(input);
    query.evaluateTo(&output);
    if(output.toDouble()!=-1){
        IDs.append(output.toDouble());
    }

    //check albums
    output = "-1";
    input = "doc('database/album.xml')/albumRoot/album[name=" + str + "]/ID/text()";
    query.setQuery(input);
    query.evaluateTo(&output);
    if(output.toDouble()!=-1){
        IDs.append(output.toDouble());
    }

    //check playlists
    output = "-1";
    input = "doc('database/playlist.xml')/playlistRoot/playlist[name=" + str + "]/ID/text()";
    query.setQuery(input);
    query.evaluateTo(&output);
    if(output.toDouble()!=-1){
        IDs.append(output.toDouble());
    }

    return IDs;
}


void Database::add_hotspot(int songID, qint64 hotspot){

    char temp[256];
    QXmlQuery query;
    QString output = "-1";
    sprintf(temp, "doc('database/hotspots.xml')/hotspotRoot/hotspot[songID=%d and spot=%d]/songID/text()", songID, hotspot);
    query.setQuery(temp);
    query.evaluateTo(&output);
    if(output.toDouble()!=songID){
        QDomElement hotspotE = hotspots.createElement("hotspot");
        hotspots.elementsByTagName("hotspotRoot").at(0).appendChild(hotspotE);

        QDomElement songIDE = hotspots.createElement("songID");
        sprintf(temp, "%d", songID);
        QDomText songIDT = hotspots.createTextNode(temp);
        songIDE.appendChild(songIDT);
        hotspotE.appendChild(songIDE);

        QDomElement spotE = hotspots.createElement("spot");
        sprintf(temp, "%d", hotspot);
        QDomText spotT = hotspots.createTextNode(temp);
        spotE.appendChild(spotT);
        hotspotE.appendChild(spotE);

        saveFile(hotspots, "database/hotspots.xml");
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
    hotspots.setContent(&write_file);
    write_file.close();
}

void Database::new_playlist(const QString &name, int ID, bool is_catalog){

    qDebug() << "Database: " << name << endl;
    char temp[256];
    QXmlQuery query;
    QString output = "-1";
    sprintf(temp, "doc('database/playlist.xml')/playlistRoot/playlist[ID=%d]/ID/text()", ID);
    query.setQuery(temp);
    query.evaluateTo(&output);

    if(output.toDouble()!=ID){
        QDomElement playlistE = playlist.createElement("playlist");
        playlist.elementsByTagName("playlistRoot").at(0).appendChild(playlistE);

        QDomElement IDE = playlist.createElement("ID");
        sprintf(temp, "%d", ID);
        QDomText IDT = playlist.createTextNode(temp);
        IDE.appendChild(IDT);
        playlistE.appendChild(IDE);

        if (is_catalog)
        {
            QDomElement is_cat_E = playlist.createElement("is_catalog");
            QDomText is_cat_T = playlist.createTextNode("true");
            is_cat_E.appendChild(is_cat_T);
            playlistE.appendChild(is_cat_E);
        }

        QDomElement nameE = playlist.createElement("name");
        QDomText nameT = playlist.createTextNode(name);
        nameE.appendChild(nameT);
        playlistE.appendChild(nameE);

        saveFile(playlist, "database/playlist.xml");
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
    playlist.setContent(&write_file);
    write_file.close();

    //insert into playlist.xml with ID and name
    char temp[256];
    QDomElement playlistE = playlist.createElement("playlist");
    playlist.elementsByTagName("playlistRoot").at(0).appendChild(playlistE);

    QDomElement listIDE = playlist.createElement("ID");
    sprintf(temp, "%d", listID);
    QDomText listIDT = playlist.createTextNode(temp);
    listIDE.appendChild(listIDT);
    playlistE.appendChild(listIDE);

    QDomElement nameE = playlist.createElement("name");
    sprintf(temp, "%d", newName.toStdString().c_str());
    QDomText nameT = playlist.createTextNode(temp);
    nameE.appendChild(nameT);
    playlistE.appendChild(nameE);

    saveFile(playlist, "database/playlist.xml");
}

void Database::add_to_playlist(int songID, int listID){

    char temp[256];
    QXmlQuery query;
    QString output = "-1";
    cout << "Adding " << songID << " to " << listID << endl;
    sprintf(temp, "doc('database/songsInPlaylist.xml')/SIPRoot/songInPlaylist[songID=%d and playlistID=%d]/songID/text()", songID, listID);
    query.setQuery(temp);
    query.evaluateTo(&output);
    if(output.toInt()!=songID){
        //relation not already there
        QDomElement SIPE = songsInPlaylist.createElement("songInPlaylist");
        songsInPlaylist.elementsByTagName("SIPRoot").at(0).appendChild(SIPE);

        QDomElement SIDE = songsInPlaylist.createElement("songID");
        sprintf(temp, "%d", songID);
        QDomText SIDT = songsInPlaylist.createTextNode(temp);
        SIDE.appendChild(SIDT);
        SIPE.appendChild(SIDE);

        QDomElement LIDE = songsInPlaylist.createElement("playlistID");
        sprintf(temp, "%d", listID);
        QDomText LIDT = songsInPlaylist.createTextNode(temp);
        LIDE.appendChild(LIDT);
        SIPE.appendChild(LIDE);

        saveFile(songsInPlaylist, "database/songsInPlaylist.xml");
    }
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
    playlist.setContent(&write_file);
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
    songsInPlaylist.setContent(&write_file);
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
    songsInPlaylist.setContent(&write_file);
    write_file.close();


}
