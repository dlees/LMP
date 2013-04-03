#include "media_manager.h"

#include "song.h"

Media_Manager::Media_Manager()
{
    playing.replace_song(new Song("C:/Friday.mp3"));
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
    playing.replace_song(new Song(filename));
    playing.play();
}

void Media_Manager::play_new(Song *song)
{

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
