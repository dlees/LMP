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
    Song(const QString &name_, int id,
         const QString &filename_, int seconds_,
         int rating_, QDateTime created,
         const QString &artist_, const QString &album_);

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

    virtual void stop_playing();
    virtual void start_playing();

signals:
    // anytime our data is changed
    // sec_count++, metadata set, is playing or not.
    void data_changed();

private slots:
    void set_song_data(Phonon::State, Phonon::State);

private:
    QString filename;
    int seconds;
    int rating; // rating out of 5
    QDateTime created;

    Phonon::MediaObject *mediaObject;

    QString artist;
    QString album;


    //    Album *album; // consider list
    //    QList<Playlist *> playlists; //could be useful

};

#endif // SONG_H
