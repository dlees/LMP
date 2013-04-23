#include "media_manager.h"

#include "song.h"
#include "library.h"
#include "playlist.h"
#include "collection.h"

Media_Manager::Media_Manager()
    : playing(Playing_Song::get())
{
    cur_list = lib.get_all_songs();
    cur_list->start_playing();

    connect(&playing, SIGNAL(became_paused()),
            this, SLOT(became_paused_slot()));
    connect(&playing, SIGNAL(started_playing()),
            this, SLOT(started_playing_slot()));
    connect(&playing, SIGNAL(AlmostDone_PlaySong()),
            this, SLOT(next()));
}

void Media_Manager::play_cur()
{
    if (playing.is_paused())
        playing.play();
    else
        playing.pause();
}

void Media_Manager::start_up()
{    /*
    Song *new_song;

       cur_list->add(new_song = lib.get_song("/Users/arikk/Music/80's - Billy Joel - We Didn't Start the Fire.mp3"));
       playing.replace_song(new_song);

       cur_list->add(lib.get_song("/Users/arikk/Music/E-40 - Tell Me When To Go (Trackademics remix).mp3"));

       cur_list->add(lib.get_song("/Users/arikk/Music/Eagle Eyed Cherry - Save Tonight.mp3"));

       cur_list->add(lib.get_song("/Users/arikk/Music/Guns N' Roses -  Sweet Child Of Mine.mp3"));

       cur_list->set_cur(0);
*/
     center = cur_list;
}

void Media_Manager::play_new(QString filename)
{
    Song *new_song = lib.get_song(filename);

    play_new(new_song);
}

void Media_Manager::play_new(Song *song)
{    
    playing.replace_song(song);
    playing.play();
}

void Media_Manager::first()
{
    cur_list->set_cur(-1);
    cur_list->next();
}

void Media_Manager::prev()
{
    cur_list->prev();
}

void Media_Manager::next()
{
    cur_list->next();
}

void Media_Manager::add_cur_to_playlist()
{
    add_to_playlist(playing.get_cur_song());
}

void Media_Manager::add_to_playlist(Song *song)
{
    cur_list->add(song);
}

void Media_Manager::switch_playlist(Collection *col)
{
    switch_playlist(new Playlist(col));
}

void Media_Manager::switch_playlist(Playlist *playlist, int pos)
{
    // tell the old list that it isn't currently playing anymore
    if (cur_list)
        cur_list->stop_playing();

    cur_list = playlist;
    cur_list->set_cur(pos);

    // tell the new one that it is playing
    cur_list->start_playing();

    emit playlist_changed(cur_list);

    // if we aren't playing a song
    //if (playing.is_paused())
        first(); // go to the first song in the playlist
}

void Media_Manager::new_playlist()
{
    lib.add_playlist(new Playlist());
}

void Media_Manager::new_playlist(QStringList *filenames, QString name)
{
    Playlist *new_pl = new Playlist(name);
    QString filename;

    foreach (filename, *filenames)
    {
        new_pl->add(lib.get_song(filename));
    }

    lib.add_playlist(new_pl);

    set_center(new_pl);
//    switch_playlist(new_pl);
}


void Media_Manager::set_center(Collection *new_center)
{
    center = new_center;

    emit center_changed(new_center);
}

Collection *Media_Manager::get_playlist()
{
    return cur_list;
}

Collection *Media_Manager::get_library()
{
    return &lib;
}

Collection *Media_Manager::get_center()
{
    return center;
}
