#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <QString>

#include <QObject>

#include "playing_song.h"
#include "library.h"

class Song;
class Playlist;
class Collection;

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

    void start_up();

    void play_new(QString filename);
    void play_new(Song *song);

    void first();
    void next();
    void prev();

    void add_cur_to_playlist();
    void add_to_playlist(Song *);
    void switch_playlist(Collection *);
    void switch_playlist(Playlist *);

    Collection *get_playlist();
    Collection *get_center();
    Collection *get_library();

    void set_center(Collection *new_center)
        { center = new_center;}


    void became_paused_slot() {emit became_paused();}
    void started_playing_slot() {emit started_playing();}

signals:
    void became_paused();
    void started_playing();

    void playlist_updated();
    void library_updated();
    void current_position(int);

private:
    Media_Manager();

    Playlist *cur_list;
    Playing_Song &playing;
    Library lib;
    Collection *center;
};

#endif // MEDIA_MANAGER_H
