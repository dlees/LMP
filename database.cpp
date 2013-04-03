#include "database.h"

#include <QDateTime>
#include <QDebug>


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
}

void Database::add_song(int ID, const QString &filename, const QDateTime& created)
{
    qDebug() << "At" << created << filename << "added to DB with ID =" << ID ;
}
