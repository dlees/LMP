#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>
#include <QList>
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

    void update_song_name(int ID, const QString &new_name);

    void add_song(int ID, const QString &filename, const QString &name, const QDateTime &created,
                  const QString &artistName = "noName", const QString &albumName = "noAName", int albumID = 43, int artistID = 42);
    void delete_song(int ID);


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

    void new_playlist(const QString &name, int ID, bool is_catalog);
    void delete_playlist(int listID);

    void add_to_playlist(int songID, int listID);
    void delete_from_playlist(int songID, int listID);

    void edit_playlist_name(int listID, QString newName);

    QList<SongInfo> *get_all_song_info();
    QList<PlaylistInfo> *get_all_list_info();

    void setup_rating_count();
    void save_rating_count(int ID, int rating);

    void save_songs();

    // saves current timestamp in a field called "timestamp"
    void save_cur_timestamp(QDomDocument &document, QDomElement &entry);
    QDomElement create_entry(QDomDocument &document, const QString &entry_name, const QString &root_name);

    void load_file(const std::string &filename, QDomDocument &document, const QString &root_name);

    void parse_song_db(QList<SongInfo> *song_list);
    void parse_rating_db(Handle_Count *handle_count);
    void parse_sec_count_db(Handle_Count *handle_count);
    void save_sec_count_threaded(int ID, qint64 start, qint64 end);
    void map_id_to_seconds(int endTime, QMap<int, int> &songIDToSeconds, int startTime, int ID);

    DataList *get_sec_count_data();
    void save_rating_count_threaded(int ID, int rating);
    void save_changed_dbs();
private:
    QDomDocument song;
    QDomDocument playlist;
    QDomDocument secCount;
    QDomDocument songsInPlaylist;
    QDomDocument hotspots;
    QDomDocument ratingCount;

    Database();
    void saveFile(QDomDocument &, const QString &);

    bool songs_db_changed;
    bool song_in_playlist_changed;
    bool playlist_changed;

};

#endif // DATABASE_H
