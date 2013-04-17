#include "media_manager.h"

#include "song.h"
#include "library.h"
#include "playlist.h"
#include "collection.h"

Media_Manager::Media_Manager()
    : cur_list(new Playlist()),
      playing(Playing_Song::get())
{
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
{
    lib.add_playlist(cur_list);
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
    cur_list = playlist;
    cur_list->set_cur(pos);

    emit playlist_changed(cur_list);

    // if we aren't playing a song
    if (playing.is_paused())
        first(); // go to the first song in the playlist
}

void Media_Manager::new_playlist()
{
    lib.add_playlist(new Playlist());
}

void Media_Manager::new_playlist(QStringList *filenames)
{
    Playlist *new_pl = new Playlist();
    QString filename;

    foreach (filename, *filenames)
    {
        new_pl->add(lib.get_song(filename));
    }

    lib.add_playlist(new_pl);

    switch_playlist(new_pl);
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
