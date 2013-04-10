#include "database.h"

#include <QDateTime>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/qxmlquery.h>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

void Database::saveFile(QDomDocument document, QString filename){

    QFile outFile(filename);
    outFile.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream stream( &outFile );
    stream << document.toString();

    outFile.close();
    outFile.open(QIODevice::ReadOnly|QIODevice::Text);
    document.setContent(&outFile);
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
        QDomText text = song.createTextNode(" ");
        root.appendChild(text);
        saveFile(song, "database/song.xml");
    }

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
        QDomElement root = song.createElement("playlistRoot");
        playlist.appendChild(root);
        saveFile(playlist, "database/playlist.xml");
    }

    //Album
    ifstream albumStream("database/album.xml");
    if(!albumStream){
        //create file
        temp.open("database/album.xml");
        temp.close();
    }
    //load file
    QFile albumFile("database/album.xml");
    song.setContent(&albumFile);
    if(!albumStream){
        QDomElement root = album.createElement("albumRoot");
        album.appendChild(root);
        saveFile(album, "database/album.xml");
    }

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

    //SecCount
    ifstream secStream("database/secCount.xml");
    if(!secStream){
        temp.open("database/secCount.xml");
        temp.close();
    }
    //load file
    QFile secFile("database/secCount.xml");
    secCount.setContent(&secFile);
    if(!secStream){
        QDomElement root = secCount.createElement("secRoot");
        secCount.appendChild(root);
        saveFile(secCount, "database/secCount.xml");
    }

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

    /*	---Text file example---
     * 	721 (the id for Friday.mp3)
     * 	0	17000
     * 	Tue Sep 25 00:11:34 2012
     * 	|~|
     */

    char temp[32];  //for fake itoa

    QDomElement secCountE = secCount.createElement("secCount");
    secCount.elementsByTagName("secRoot").at(0).appendChild(secCountE);

    QDomElement idE = secCount.createElement("ID");
    sprintf(temp, "%d", ID);
    QDomText idT = secCount.createTextNode(temp);
    idE.appendChild(idT);
    secCountE.appendChild(idE);

    QDomElement startE = secCount.createElement("startTime");
    sprintf(temp, "%d", start);
    QDomText startT = secCount.createTextNode(temp);
    startE.appendChild(startT);
    secCountE.appendChild(startE);

    QDomElement endE = secCount.createElement("endTime");
    sprintf(temp, "%d", end);
    QDomText endT = secCount.createTextNode(temp);
    endE.appendChild(endT);
    secCountE.appendChild(endE);

    time_t rawTime;
    struct tm * timeinfo;
    time(&rawTime);
    QDomElement timeE = secCount.createElement("timestamp");
    timeinfo = localtime(&rawTime);
    QDomText timeT = secCount.createTextNode(asctime(timeinfo));
    timeE.appendChild(timeT);
    secCountE.appendChild(timeE);

    saveFile(secCount, "database/secCount.xml");
}

void Database::add_song(int songID, const QString &filename,
                        const QString &name, const QDateTime& created)
{
    qDebug() << "At" << created << filename << "added to DB with ID =" << songID ;

    char temp[64];

    //temp data until real stuff gets put in
    QString artistName = "XTC";
    QString albumName = "Apple Venus Volume One";
    int albumID = 56345;
    int artistID = 98734;

    //QXmlQuery query;
    //QString output = "notRight";
    //sprintf(temp, "doc('database/song.xml')/songRoot/song[ID=%d]/ID", songID);
    //query.setQuery(temp);
    //query.evaluateTo(&output);
    if(/*!(output==QString::number(songID))*/true){
        //Song
        QFile file;

       // song.setContent("database/song.xml");
        QDomElement songE = song.createElement("song");
        QDomElement root = song.documentElement().elementsByTagName("songRoot").at(0).toElement();
        if(root.isNull()){
            cout << "null null null" << endl;
        }
        root.appendChild(songE);

        QDomElement IDE = song.createElement("ID");
        sprintf(temp, "%d", songID);
        QDomText IDT = song.createTextNode(temp);
        IDE.appendChild(IDT);
        songE.appendChild(IDE);

        QDomElement nameE = song.createElement("name");
        QDomText nameT = song.createTextNode(name);
        nameE.appendChild(nameT);
        song.appendChild(nameE);

        QDomElement pathE = song.createElement("filepath");
        QDomText pathT = song.createTextNode(filename);
        pathE.appendChild(pathT);
        songE.appendChild(pathE);

        QDomElement createdE = song.createElement("created");
        QDomElement createdT = song.createElement(created.toString());
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
    //sprintf(temp, "doc('database/album.xml')/albumRoot/album[ID=%d]/ID", artistID);
    //query.setQuery(temp);
    //output = "wrong";
    //query.evaluateTo(&output);
    if(/*!(output==QString::number(albumID))*/true){
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
    //sprintf(temp, "doc('database/artist.xml')/artistRoot/artist[ID=%d]/ID", artistID);
    //query.setQuery(temp);
    //output = "incorrect";
    //query.evaluateTo(&output);
    if(/*!(output==QString::number(artistID))*/true){
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

void delete_song(int ID){

}

int find_filename(const QString &filename){
    // returns the ID of a song that has filename as its filename
    // -1 if filename doesn't exit
    return 0;
}

QList<int> find(const QString &str){
    // returns all the id's of anything in the database
    // that has str in it (Songs, Artists, Albums, Playlists)
    return QList<int>();
}

void newPlaylist(const QString &name){

}

void add_to_playlist(int songID, int listID){

}

void delete_from_playlist(int songID, int listID){

}
