#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>
#include <QList>
#include "XML/tinyxml.h"

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

    void add_song(int ID, const QString &filename,
                  const QString &name, const QDateTime &created);

    void delete_song(int ID);

    // returns the ID of a song that has filename as its filename
    // -1 if filename doesn't exit
    int find_filename(const QString &filename);

    // returns all the id's of anything in the database
    // that has str in it (Songs, Artists, Albums, Playlists)
    // **NOTE: All Music Items have unique ID's, so there won't be conflictions
    QList<int> find(const QString &str);

    void newPlaylist(const QString &name);

    void add_to_playlist(int songID, int listID);

    void delete_from_playlist(int songID, int listID);

private:
    TiXmlDocument song;
    TiXmlDocument playlist;
    TiXmlDocument album;
    TiXmlDocument artist;
    TiXmlDocument secCount;
    TiXmlDocument songsInPlaylist;
    TiXmlDocument songsOnAlbum;
    TiXmlDocument albumsByArtist;

    Database();
};

#endif // DATABASE_H
