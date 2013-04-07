#include "database.h"

#include <QDateTime>
#include <QDebug>
#include <fstream>
#include <ctime>
#include "XML/xpath_static.h"

using namespace std;

Database::Database(){
    //load or create all files/tables

    //Song
    ifstream songStream("database/song.xml");
    if(!songStream){
        //create file
        ofstream temp("database/song.xml");
    }
    //load file
    song.LoadFile("database/song.xml");

    //Playlist
    ifstream playlistStream("database/playlist.xml");
    if(!playlistStream){
        //create file
        ofstream temp("database/playlist.xml");
    }
    //load file
    playlist.LoadFile("databse/playlist.xml");

    //Album
    ifstream albumStream("database/album.xml");
    if(!albumStream){
        //create file
        ofstream temp("database/album.xml");
    }
    //load file
    album.LoadFile("database/album.xml");

    //Artist
    ifstream artistStream("database/artist.xml");
    if(!artistStream){
        //create file
        ofstream temp("database/artist.xml");
    }
    //load file
    artist.LoadFile("database/artist.xml");

    //SecCount
    ifstream secStream("database/secCount.xml");
    if(!secStream){
        //create file
        ofstream temp("database/secCount.xml");
    }
    //load file
    secCount.LoadFile("database/secCount.xml");

    //SongsInPlaylist
    ifstream SIPStream("database/songsInPlaylist.xml");
    if(!SIPStream){
        //create file
        ofstream temp("database/songsInPlaylist.xml");
    }
    //load file
    songsInPlaylist.LoadFile("database/songsInPlaylist.xml");

    //SongsOnAlbum
    ifstream SOAStream("database/songsOnAlbum.xml");
    if(!SOAStream){
        //create file
        ofstream temp("database/songsOnAlbum.xml");
    }
    //load file
    songsOnAlbum.LoadFile("database/songsOnAlbum.xml");

    //AlbumsByArtist
    ifstream ABAStream("database/albumsByArtist.xml");
    if(!ABAStream){
        //create file
        ofstream temp("database/albumsByArtist.xml");
    }
    //load file
    albumsByArtist.LoadFile("database/albumsByArtist.xml");


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

    TiXmlElement *secCountE = new TiXmlElement("secCount");
    secCount.LinkEndChild(secCountE);

    TiXmlElement *idE = new TiXmlElement("ID");
    sprintf(temp, "%d", ID);
    TiXmlText *idT = new TiXmlText(temp);
    idE->LinkEndChild(idT);
    secCountE->LinkEndChild(idE);

    TiXmlElement *startE = new TiXmlElement("startTime");
    sprintf(temp, "%d", start);
    TiXmlText *startT = new TiXmlText(temp);
    startE->LinkEndChild(startT);
    secCountE->LinkEndChild(startE);

    TiXmlElement *endE = new TiXmlElement("endTime");
    sprintf(temp, "%d", end);
    TiXmlText *endT = new TiXmlText(temp);
    endE->LinkEndChild(endT);
    secCountE->LinkEndChild(endE);

    time_t rawTime;
    struct tm * timeinfo;
    time(&rawTime);
    TiXmlElement *timeE = new TiXmlElement("timestamp");
    timeinfo = localtime(&rawTime);
    TiXmlText *timeT = new TiXmlText(asctime(timeinfo));
    timeE->LinkEndChild(timeT);
    secCountE->LinkEndChild(timeE);

    secCount.SaveFile();
}

void Database::add_song(int songID, const QString &filename,
                        const QString &name, const QDateTime& created)
{
    qDebug() << "At" << created << filename << "added to DB with ID =" << songID ;

    char temp[32];

    //temp data until real stuff gets put in
    QString artistName = "XTC";
    QString albumName = "Apple Venus Volume One";
    int albumID = 56345;
    int artistID = 98734;

    if(true/*TODO if song not already in database*/){

        //Song
        TiXmlElement *songE = new TiXmlElement("song");
        song.LinkEndChild(songE);

        TiXmlElement *IDE = new TiXmlElement("ID");
        sprintf(temp, "%d", songID);
        TiXmlText *IDT = new TiXmlText(temp);
        IDE->LinkEndChild(IDT);
        songE->LinkEndChild(IDE);

        TiXmlElement *nameE = new TiXmlElement("name");
        TiXmlText *nameT = new TiXmlText(name.toStdString().c_str());
        nameE->LinkEndChild(nameT);
        songE->LinkEndChild(nameE);

        TiXmlElement *pathE = new TiXmlElement("filepath");
        TiXmlText *pathT = new TiXmlText(filename.toStdString().c_str());
        pathE->LinkEndChild(pathT);
        songE->LinkEndChild(pathE);

        TiXmlElement *createdE = new TiXmlElement("created");
        TiXmlText *createdT = new TiXmlText(created.toString().toStdString().c_str());
        createdE->LinkEndChild(createdT);
        songE->LinkEndChild(createdE);

        song.SaveFile();

        //songOnAlbum
        TiXmlElement *onAlbumE = new TiXmlElement("songOnAlbum");
        songsOnAlbum.LinkEndChild(onAlbumE);

        TiXmlElement *songIDE = new TiXmlElement("songID");
        sprintf(temp, "%d", songID);
        TiXmlText *songIDT = new TiXmlText(temp);
        songIDE->LinkEndChild(songIDT);
        onAlbumE->LinkEndChild(songIDE);

        TiXmlElement *albumIDE = new TiXmlElement("albumID");
        sprintf(temp, "%d", albumID);
        TiXmlText *albumIDT = new TiXmlText(temp);
        albumIDE->LinkEndChild(albumIDT);
        onAlbumE->LinkEndChild(albumIDE);

        songsOnAlbum.SaveFile();
    }
    if(true /*album not already in database*/){

        //album
        TiXmlElement *albumE = new TiXmlElement("album");
        album.LinkEndChild(albumE);

        TiXmlElement *nameE = new TiXmlElement("name");
        TiXmlText *nameT = new TiXmlText(albumName.toStdString().c_str());
        nameE->LinkEndChild(nameT);
        albumE->LinkEndChild(nameE);

        TiXmlElement *IDE = new TiXmlElement("ID");
        sprintf(temp, "%d", albumID);
        TiXmlText *IDT = new TiXmlText(temp);
        IDE->LinkEndChild(IDT);
        albumE->LinkEndChild(IDE);

        album.SaveFile();

        //AlbumByArtist
        TiXmlElement *byArtistE = new TiXmlElement("albumByArtist");
        albumsByArtist.LinkEndChild(byArtistE);

        TiXmlElement *albumIDE = new TiXmlElement("albumID");
        sprintf(temp, "%d", albumID);
        TiXmlText *albumIDT = new TiXmlText(temp);
        albumIDE->LinkEndChild(albumIDT);
        byArtistE->LinkEndChild(albumIDE);

        TiXmlElement *artistIDE = new TiXmlElement("artistID");
        sprintf(temp, "%d", artistID);
        TiXmlText *artistIDT = new TiXmlText(temp);
        artistIDE->LinkEndChild(artistIDT);
        byArtistE->LinkEndChild(artistIDE);

        albumsByArtist.SaveFile();
    }
    if(true /*artist not already in database*/){
        //artist
        TiXmlElement *artistE = new TiXmlElement("artist");
        artist.LinkEndChild(artistE);

        TiXmlElement *nameE = new TiXmlElement("name");
        TiXmlText *nameT = new TiXmlText(artistName.toStdString().c_str());
        nameE->LinkEndChild(nameT);
        artistE->LinkEndChild(nameE);

        TiXmlElement *IDE = new TiXmlElement("ID");
        sprintf(temp, "%d", artistID);
        TiXmlText *IDT = new TiXmlText(temp);
        IDE->LinkEndChild(IDT);
        artistE->LinkEndChild(IDE);

        artist.SaveFile();
    }
}
