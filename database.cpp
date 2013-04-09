#include "database.h"

#include <QDateTime>
#include <QDebug>
#include <fstream>
#include <ctime>
#include <iostream>
#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include "XML/xpath_static.h"

using namespace std;

Database::Database(){
    //load or create all files/tables

    ofstream temp;
    boost::filesystem::create_directories("database");

    //Song
    ifstream songStream("database/song.xml");
    if(!songStream){
        temp.open("database/song.xml");
        temp.close();
    }
    //load file
    song.LoadFile("database/song.xml");
    if(!songStream){
        TiXmlElement *songRoot = new TiXmlElement("songRoot");
        song.LinkEndChild(songRoot);
        song.SaveFile();
    }

    //Playlist
    ifstream playlistStream("database/playlist.xml");
    if(!playlistStream){
        //create file
        temp.open("database/playlist.xml");
        temp.close();
    }
    //load file
    playlist.LoadFile("databse/playlist.xml");
    if(!playlistStream){
        TiXmlElement *playRoot = new TiXmlElement("playlistRoot");
        playlist.LinkEndChild(playRoot);
        playlist.SaveFile();
    }

    //Album
    ifstream albumStream("database/album.xml");
    if(!albumStream){
        //create file
        temp.open("database/album.xml");
        temp.close();
    }
    //load file
    album.LoadFile("database/album.xml");
    if(!albumStream){
        TiXmlElement *albumRoot = new TiXmlElement("albumRoot");
        album.LinkEndChild(albumRoot);
        album.SaveFile();
    }

    //Artist
    ifstream artistStream("database/artist.xml");
    if(!artistStream){
        temp.open("database/artist.xml");
        temp.close();
    }
    //load file
    artist.LoadFile("database/artist.xml");
    if(!artistStream){
        TiXmlElement *artistRoot = new TiXmlElement("artistRoot");
        artist.LinkEndChild(artistRoot);
        artist.SaveFile();
    }

    //SecCount
    ifstream secStream("database/secCount.xml");
    if(!secStream){
        temp.open("database/secCount.xml");
        temp.close();
    }
    //load file
    secCount.LoadFile("database/secCount.xml");
    if(!secStream){
        TiXmlElement *secRoot = new TiXmlElement("secRoot");
        secCount.LinkEndChild(secRoot);
        secCount.SaveFile();
    }

    //SongsInPlaylist
    ifstream SIPStream("database/songsInPlaylist.xml");
    if(!SIPStream){
        temp.open("database/songsInPlaylist.xml");
        temp.close();
    }
    //load file
    songsInPlaylist.LoadFile("database/songsInPlaylist.xml");
    if(!SIPStream){
        TiXmlElement *SIPRoot = new TiXmlElement("SIPRoot");
        songsInPlaylist.LinkEndChild(SIPRoot);
        songsInPlaylist.SaveFile();
    }

    //SongsOnAlbum
    ifstream SOAStream("database/songsOnAlbum.xml");
    if(!SOAStream){
        temp.open("database/songsOnAlbum.xml");
        temp.close();
    }
    //load file
    songsOnAlbum.LoadFile("database/songsOnAlbum.xml");
    if(!SOAStream){
        TiXmlElement *SOARoot = new TiXmlElement("SOARoot");
        songsOnAlbum.LinkEndChild(SOARoot);
        songsOnAlbum.SaveFile();
    }

    //AlbumsByArtist
    ifstream ABAStream("database/albumsByArtist.xml");
    if(!ABAStream){
        temp.open("database/albumsByArtist.xml");
        temp.close();
    }
    //load file
    albumsByArtist.LoadFile("database/albumsByArtist.xml");
    if(!ABAStream){
        TiXmlElement *ABARoot = new TiXmlElement("ABARoot");
        albumsByArtist.LinkEndChild(ABARoot);
        albumsByArtist.SaveFile();
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

    TiXmlElement *secCountE = new TiXmlElement("secCount");
    secCount.FirstChild()->LinkEndChild(secCountE);

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

    char temp[64];

    //temp data until real stuff gets put in
    QString artistName = "XTC";
    QString albumName = "Apple Venus Volume One";
    int albumID = 56345;
    int artistID = 98734;
    int ret = 0;

    sprintf(temp, "/songRoot/song[ID=%d]/ID", songID);
    cout << temp << endl;
    bool check = TinyXPath::o_xpath_int(song.FirstChild(), temp, ret);
    if(check==false){
        cout << "FAILURE" << endl;
    }
    if(ret!=songID){
        cout << ret << " " << songID <<  endl;
        //Song
        TiXmlElement *songE = new TiXmlElement("song");
        song.FirstChild()->LinkEndChild(songE);

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
        songsOnAlbum.FirstChild()->LinkEndChild(onAlbumE);

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
    sprintf(temp, "/albumRoot/album[ID=%d]/ID/", albumID);
    TinyXPath::o_xpath_int(album.FirstChild(), temp, ret);
    if(ret!=albumID){
        cout << ret << " " << albumID << endl;
        //album
        TiXmlElement *albumE = new TiXmlElement("album");
        album.FirstChild()->LinkEndChild(albumE);

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
        albumsByArtist.FirstChild()->LinkEndChild(byArtistE);

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
    sprintf(temp, "/artistRoot/artist[ID=%d]/ID/", artistID);
    TinyXPath::o_xpath_int(artist.FirstChild(), temp, ret);
    if(ret!=artistID){
        //artist
        TiXmlElement *artistE = new TiXmlElement("artist");
        artist.FirstChild()->LinkEndChild(artistE);

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
