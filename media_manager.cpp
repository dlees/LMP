#include "media_manager.h"

#include "song.h"
#include "library.h"
#include "playlist.h"

Media_Manager::Media_Manager()
    : cur_list(new Playlist())
{
    connect(&playing, SIGNAL(became_paused()),
            this, SLOT(became_paused_slot()));
    connect(&playing, SIGNAL(started_playing()),
            this, SLOT(started_playing_slot()));


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

    play_new(new_song);
}

void Media_Manager::play_new(Song *new_song)
{    
    playing.replace_song(new_song);
    playing.play();

    lib.add_song(new_song);
    cur_list->add(new_song);


}

void Media_Manager::first()
{

}

void Media_Manager::prev()
{

}

void Media_Manager::next()
{

}

void Media_Manager::add_to_playlist()
{

}
