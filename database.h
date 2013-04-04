#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>

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

private:
    Database(){}
};

#endif // DATABASE_H
