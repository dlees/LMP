#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <QString>

#include "playing_song.h"

class Song;

/* This class is the main "Model" for the music player.
 *  MainWindow is the "View/Controller" for it.
 *  It is in charge of:
 *      playing songs
 *      keeping track of the library
 *      playing playlists
 *      manipulating the library
 */


class Media_Manager
{
public:
    Media_Manager();

    // plays the current song
    void play_cur();

    void play_new(QString filename);
    void play_new(Song *song);

    void first();
    void next();
    void prev();

private:
    Playing_Song playing;


};

#endif // MEDIA_MANAGER_H
