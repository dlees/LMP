#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>
#include <QList>
#include <QtXml/QtXml>

struct PlaylistInfo{
    QList<int> songIDs;
    int ID;
    QString name;
};

struct HotspotInfo{
    int ID;
    QList<qint64> hotspots;
};

class SongInfo{
public:
    SongInfo();
    SongInfo(const SongInfo& other);
    //ID, name, filename, created, seconds, album, artist
    int songID = 0;
    QString songName;
    QString fileName;
    QDateTime created;
    int seconds = 0;
    QString albumName;
    QString artistName;
};

class Database
{
public:
    static Database *get()
    {
        static Database *db = new Database();
        return db;
    }

    // save a sec count to the database
    void save_sec_count(int ID, qint64 start, qint64 end);

    void add_song(int ID, const QString &filename, const QString &name, const QDateTime &created,
                  const QString &artistName = "noName", const QString &albumName = "noAName", int albumID = 43, int artistID = 42);
    void delete_song(int ID);
    QList<SongInfo> *get_all_song_info();

    void add_hotspot(int songID, qint64 hotspot);
    void delete_hotspot(int songID, qint64 hotspot);
    QList<HotspotInfo> *get_hotspot_info();

    // returns the ID of a song that has filepath as its filepath
    // -1 if file doesn't exit
    int find_filepath(const QString &filename);

    // returns ID of song with 'name' as its name
    // -1 if DNE
    int find_filename(const QString &name);

    // returns all the ids of anything in the database
    // that has str in it (Songs, Artists, Albums, Playlists)
    // **NOTE: All Music Items have unique ID's, so there won't be conflictions
    QList<int> find(const QString &str);

    void new_playlist(const QString &name, int ID);
    void add_to_playlist(int songID, int listID);
    void delete_from_playlist(int songID, int listID);
    void edit_playlist_name(int listID, QString newName);
    QList<PlaylistInfo> *get_all_list_info();

private:
    QDomDocument song;
    QDomDocument playlist;
    QDomDocument album;
    QDomDocument artist;
    QDomDocument secCount;
    QDomDocument songsInPlaylist;
    QDomDocument songsOnAlbum;
    QDomDocument albumsByArtist;
    QDomDocument hotspots;

    Database();
    void saveFile(QDomDocument, QString);
};

#endif // DATABASE_H
