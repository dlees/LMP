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

    QString get_filename() const
        { return filename;}

    void add_secs(int amt);

    virtual int total_secs() const
        { return seconds; }

    virtual QLinkedList<Song *> get_leaves();
    virtual void begin_playing();

    virtual void add_child(QStandardItem *);

    // gets the appropriate headers for this music item
    virtual QStringList get_headers() const;

    virtual QList<QStandardItem*> get_column_data() const;

    void stop_playing()
        {is_playing = false;}


private:
    QString filename;
    int seconds;
    int rating; // rating out of 5
    QDateTime created;


    //    Artist *artist; //consider List
    //    Album *album; // consider list
    //    QList<Playlist *> playlists; //could be useful


    bool is_playing;
};

#endif // SONG_H
