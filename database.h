#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>
#include <QList>

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

    // returns the ID of a song that has filename as it's filename
    // -1 if filename doesn't exit
    int find_filename(const QString &filename);

    // returns all the id's of anything in the database
    // that has str in it (Songs, Artists, Albums, Playlists)
    // **NOTE: All Music Items have unique ID's, so there won't be conflictions
    QList<int> find(const QString &str);

private:
    Database(){}
};

#endif // DATABASE_H
