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

class Database;

class Media_Manager : public QObject
{
    Q_OBJECT

public:
    static Media_Manager *get()
    {
        static Media_Manager *m = new Media_Manager();
        return m;
    }

    void add_playlist_to_library(Playlist *new_pl);
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
    void switch_playlist(Playlist *, int pos = 0);

    // creates a new playlist
    void new_playlist();
    void new_playlist(QStringList*, const QString &);
    void new_playlist(const std::vector<int> &, const std::string &);

    Collection *get_playlist();
    Collection *get_center();
    Library *get_library();

    void set_center(Collection *new_center);

    void became_paused_slot() {emit became_paused();}
    void started_playing_slot() {emit started_playing();}

    Collection *create_catalog(const QString &, Database *db);
    void add_to_catalog(const QString &, Music_Item *);

    void change_rating_cur_song(int rating);

    Music_Item *get_music_item(int id);

    void move_cur_song_up();
signals:
    void became_paused();
    void started_playing();

    void playlist_changed(Collection *new_list);
    void center_changed(Collection *new_center);

    void current_position(int);

private:
    Media_Manager();

    Playlist *cur_list;
    Playing_Song &playing;
    Library lib;
    Collection *center;
};


#endif // MEDIA_MANAGER_H
