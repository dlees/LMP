#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <QString>

#include <QObject>

#include "playing_song.h"
#include "library.h"

class Song;
class Playlist;

/* This class is the main "Model" for the music player.
 *  MainWindow is the "View/Controller" for it.
 *  It is in charge of:
 *      playing songs
 *      keeping track of the library
 *      playing playlists
 *      manipulating the library
 *
 *  This is a Singleton. You can access the single
 *  instance anywhere by calling:
 *    Media_Manager::get()
 *
 *  So, to call play_cur(),
 *      Media_Manager::get()->play_cur();
 *
 *  to connect a signal to play_cur(),
 *      connect(###, ###, Media_Manager::get(), SLOT(play_cur()));
 *
 */


class Media_Manager : public QObject
{
    Q_OBJECT

public:
    static Media_Manager *get()
    {
        static Media_Manager *m = new Media_Manager();
        return m;
    }

public slots:
    // plays the current song
    void play_cur();

    void play_new(QString filename);
    void play_new(Song *song);

    void first();
    void next();
    void prev();

private:

    Media_Manager();

    Playing_Song playing;
    Library lib;
    Playlist *cur_list;
};

#endif // MEDIA_MANAGER_H
