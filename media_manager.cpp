#include "media_manager.h"

#include "song.h"
#include "library.h"
#include "playlist.h"
#include "collection.h"

Media_Manager::Media_Manager()
    : cur_list(new Playlist())
{
    connect(&playing, SIGNAL(became_paused()),
            this, SLOT(became_paused_slot()));
    connect(&playing, SIGNAL(started_playing()),
            this, SLOT(started_playing_slot()));

    cur_list->add(new Song("Friday"));
    cur_list->add(new Song("Everyday"));
    cur_list->add(new Song("Both Days"));
    cur_list->add(new Song("Today"));
    play_new("C:\\Friday.mp3");
}

void Media_Manager::play_cur()
{
    if (playing.is_paused())
        playing.play();
    else
        playing.pause();
}

void Media_Manager::play_new(QString filename)
{
    Song *new_song = new Song(filename);

    lib.add_song(new_song);

    play_new(new_song);
}

void Media_Manager::play_new(Song *new_song)
{    

    //playing.get_cur_song()->stop_playing();

    playing.replace_song(new_song);
    playing.play();

}

void Media_Manager::first()
{

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
    emit playlist_updated();
}

void Media_Manager::switch_playlist(Collection *col)
{
    switch_playlist(new Playlist(col));
}

void Media_Manager::switch_playlist(Playlist *playlist)
{
    cur_list = playlist;
}

QStandardItemModel *Media_Manager::get_playlist()
{
    return cur_list->get_tree();
}

QStandardItemModel *Media_Manager::get_library()
{
    return lib.get_tree();
}
