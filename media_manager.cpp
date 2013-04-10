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
    Song *new_song;

    cur_list->add(new_song = new Song("/Users/arikk/Music/80's - Billy Joel - We Didn't Start the Fire.mp3"));
    lib.add_song(new_song);
    playing.replace_song(new_song);

    cur_list->add(new_song = new Song("/Users/arikk/Music/E-40 - Tell Me When To Go (Trackademics remix).mp3"));
    lib.add_song(new_song);

    cur_list->add(new_song = new Song("/Users/arikk/Music/Eagle Eyed Cherry - Save Tonight.mp3"));
    lib.add_song(new_song);

    cur_list->add(new_song = new Song("/Users/arikk/Music/Guns N' Roses -  Sweet Child Of Mine.mp3"));
    lib.add_song(new_song);

    cur_list->set_cur(0);

    lib.add_playlist(cur_list);
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
    return cur_list->get_model();
}

QStandardItemModel *Media_Manager::get_library()
{
    return lib.get_model();
}
