#ifndef DATABASE_H
#define DATABASE_H

#include "xmldatabase.h"

#include <QDateTime>
#include <QList>
#include <QtXml/QtXml>

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

    void save_sec_count_threaded(int ID, qint64 start, qint64 end);
    void map_id_to_seconds(int endTime, QMap<int, int> &songIDToSeconds, int startTime, int ID);

    DataList *get_sec_count_data();
    void save_rating_count_threaded(int ID, int rating);
    void save_changed_dbs();
private:
    XmlDatabase *database;

    Database();

    bool songs_db_changed;
    bool song_in_playlist_changed;
    bool playlist_changed;

    // returns the ID of a song that has filepath as its filepath
    // -1 if file doesn't exit
    int find_filepath(const QString &filename);

    // returns ID of song with 'name' as its name
    // -1 if DNE
    int find_filename(const QString &name);

};

#endif // DATABASE_H
