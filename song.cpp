#include "song.h"

#include "database.h"
#include "media_manager.h"
#include <QString>
#include <QObject>
#ifdef __linux__
    #include <phonon/mediaobject.h>
    #include <phonon/audiooutput.h>
    #include <phonon/VolumeSlider> //#include <Phonon/VolumeSlider>
#else
    #include <Phonon>
#endif
#include <QCoreApplication>

Song::Song(const QString &filename_)
    : Music_Item(filename_),
      filename(filename_),
      seconds(10),
      rating(3),
      created(QDateTime::currentDateTime()),
      is_playing(false)

{
    mediaObject = new Phonon::MediaObject;

    mediaObject->setCurrentSource(filename_);

    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(set_song_data(Phonon::State,Phonon::State)));

    // For good Mac Support - useless on Windows
    if (mediaObject->metaData("TITLE").size())
        name = mediaObject->metaData("TITLE").at(0);

  //  Database::get()->add_song(get_id(), filename, get_name(), created);
}

void Song::add_secs(int amt)
{
    seconds += amt;
}

QLinkedList<Song*> Song::get_leaves()
{
    QLinkedList<Song*> list;
    list.push_back(this);
    return list;
}

void Song::begin_playing()
{
    Media_Manager::get()->play_new(this);
    is_playing = true;
}

QStringList Song::get_headers() const
{
    return Music_Item::get_headers()
            << "Artist";
}

QList<QVariant> Song::get_column_data() const
{
    return Music_Item::get_column_data()
            << artist
               ;
}

// set the metadata of the song here
void Song::set_song_data(Phonon::State, Phonon::State oldstate)
{
    // if we aren't loading anymore
    if (oldstate != Phonon::LoadingState)
        return;

    if (mediaObject->metaData("TITLE").size())
        name = mediaObject->metaData("TITLE").at(0);

    if (mediaObject->metaData("ARTIST").size())
        artist = mediaObject->metaData("ARTIST").at(0);

    delete mediaObject;
}

