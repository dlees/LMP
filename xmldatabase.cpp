#include "xmldatabase.h"

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


using namespace std;

void saveFile(QDomDocument &document, const string &filename){

    QFile outFile(QString(filename.c_str()));
    outFile.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream stream( &outFile );
    stream << document.toString();

    outFile.close();
    outFile.open(QIODevice::ReadOnly|QIODevice::Text);
    document.setContent(&outFile);
}

void load_file(const string &filename, QDomDocument &document, const QString &root_name)
{
    ifstream fin(filename.data());
    if(!fin){
        ofstream temp;
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

XmlDatabase::XmlDatabase(const string &root_path_) :
    root_path(root_path_)
{
    load_file(root_path + "/song.xml", song, "songRoot");

    load_file(root_path + "/secCount.xml", secCount, "secRoot");

    load_file(root_path + "/ratingCount.xml", ratingCount, "rateRoot");

    load_file(root_path + "/hotspots.xml", hotspots, "hotspotRoot");

    load_file(root_path + "/playlist.xml", playlist, "playlistRoot");

    load_file(root_path + "/songsInPlaylist.xml", songsInPlaylist, "SIPRoot");
}

QList<int> XmlDatabase::find(const QString &str){
    // returns all the id's of anything in the database
    // that has str in it (Songs, Artists, Albums, Playlists)

    QList<int> IDs;
    QXmlQuery query;
    QString input;
    QString output;

    //check songs
    output = "-1";
    input = "doc('" + QString(root_path.c_str()) + "/song.xml')/songRoot/song[name=" + str + "]/ID/text()";
    query.setQuery(input);
    query.evaluateTo(&output);
    if(output.toDouble()!=-1){
        IDs.append(output.toDouble());
    }

    //check playlists
    output = "-1";
    input = "doc(" + QString(root_path.c_str()) + "/playlist.xml')/playlistRoot/playlist[name=" + str + "]/ID/text()";
    query.setQuery(input);
    query.evaluateTo(&output);
    if(output.toDouble()!=-1){
        IDs.append(output.toDouble());
    }

    return IDs;
}

#include "handle_data_value.h"

void XmlDatabase::parse_rating_db(Handle_Count *handle_count)
{
    QFile *ratingFile = new QFile(QString(root_path.c_str()) + "/ratingCount.xml");
    ratingFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader *ratingReader = new QXmlStreamReader(ratingFile);

    int ID = 0;
    int rating = 0;
    QString temp;

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
                rating = (temp.toInt());
            }
            if (ratingReader->name() == "timestamp") {
                temp = ratingReader->readElementText();

                // This is the last section of the count, so handle our data:
                handle_count->handle(ID, DataValue::get_instance(rating), temp);
            }
        }
    }
    ratingFile->close();
    ratingReader->clear();
    delete ratingFile;
    delete ratingReader;
}

void XmlDatabase::parse_sec_count_db(Handle_Count *handle_count)
{
    QFile *secFile = new QFile(QString(root_path.c_str()) + "/secCount.xml");
    secFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader *secReader = new QXmlStreamReader(secFile);
    int ID = 0;
    QString temp;
    int startTime = 0;
    int endTime = 0;

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
                endTime = temp.toInt();
            }
            if (secReader->name() == "timestamp") {
                temp = secReader->readElementText();

                // This is the last section of the count, so handle our data:
                handle_count->handle(ID, DataValue::get_instance(startTime, endTime), temp);
            }
        }
    }
    secFile->close();
    secReader->clear();
    delete secFile;
    delete secReader;
}


QMap<int, SongInfo> XmlDatabase::parse_song_db()
{
    SongInfo *curSong = 0;
    QString temp;

    QFile *songFile = new QFile(QString(root_path.c_str()) + "/song.xml");
    songFile->open(QIODevice::ReadOnly | QIODevice::Text);

    QMap<int, SongInfo> songIDtoInfo;

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
                songIDtoInfo.insert(curSong->songID, *curSong);

            }
        }
    }
    songFile->close();
    songReader->clear();
    delete songFile;
    delete songReader;

    return songIDtoInfo;
}
