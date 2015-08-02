#ifndef XMLDATABASE_H
#define XMLDATABASE_H

#include <QtXml/QtXml>

struct PlaylistInfo{
    QList<int> songIDs;
    int ID;
    QString name;
    bool is_catalog;
    PlaylistInfo() : is_catalog(false){}
};

struct HotspotInfo{
    int ID;
    QList<qint64> hotspots;
};

class SongInfo{
public:
    SongInfo();
    //ID, name, filename, created, seconds, album, artist
    int songID;
    QString songName;
    QString fileName;
    QDateTime created;
    int seconds;
    int rating;
    QString albumName;
    QString artistName;
};

class Handle_Count;
class DataList;

class XmlDatabase
{
public:
    XmlDatabase(const std::string &root_path);

    // returns all the ids of anything in the database
    // that has str in it (Songs, Artists, Albums, Playlists)
    // **NOTE: All Music Items have unique ID's, so there won't be conflictions
    QList<int> find(const QString &str);

//private:
    std::string root_path;

    QDomDocument song;
    QDomDocument playlist;
    QDomDocument secCount;
    QDomDocument songsInPlaylist;
    QDomDocument hotspots;
    QDomDocument ratingCount;

    QMap<int, SongInfo> parse_song_db();
    void parse_rating_db(Handle_Count *handle_count);
    void parse_sec_count_db(Handle_Count *handle_count);

};

#endif // XMLDATABASE_H
