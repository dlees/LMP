#ifndef SONG_H
#define SONG_H

#include "music_item.h"

#include <QString>
#include <QList>
#include <QDateTime>

#ifdef __linux__
    #include <phonon/mediaobject.h>
    #include <phonon/audiooutput.h>
    #include <phonon/VolumeSlider> //#include <Phonon/VolumeSlider>
#else
    #include <Phonon>
#endif

class Song : public QObject, public Music_Item
{
    Q_OBJECT
public:
    Song(const QString &filename_);

    QString get_filename() const
        { return filename;}

    void add_secs(int amt);

    virtual int total_secs() const
        { return seconds; }

    virtual QLinkedList<Song *> get_leaves();
    virtual void begin_playing();

    // gets the appropriate headers for this music item
    virtual QStringList get_headers() const;

    virtual QList<QVariant> get_column_data() const;

    void stop_playing()
        {is_playing = false;}

private slots:
    void set_song_data(Phonon::State, Phonon::State);

private:
    QString filename;
    int seconds;
    int rating; // rating out of 5
    QDateTime created;

    Phonon::MediaObject *mediaObject;

    QString artist;


    //    Album *album; // consider list
    //    QList<Playlist *> playlists; //could be useful


    bool is_playing;
};

#endif // SONG_H
