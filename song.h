#ifndef SONG_H
#define SONG_H

#include "music_item.h"

#include <QString>
#include <QList>
#include <QDateTime>

class Song : public Music_Item
{
public:
    Song(const QString &filename_);

    QString get_filename()
        { return filename;}

    void add_secs(int amt);

    virtual int total_secs()
        { return seconds; }


    virtual QLinkedList<Song *> get_leaves();


private:
    QString filename;
    int seconds;
    int rating; // rating out of 5
    QDateTime created;

    //    Artist *artist; //consider List
    //    Album *album; // consider list
    //    QList<Playlist *> playlists; //could be useful

};

#endif // SONG_H
