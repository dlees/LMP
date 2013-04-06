#include "playlist.h"

#include "song.h"

Playlist::Playlist()
    : Collection("Unsaved Playlist")
{
}

void Playlist::set_cur(int i)
{

}

void Playlist::add(Music_Item *item)
{
    QLinkedList<Song*> songs = item->get_leaves();
    Song* song;

    foreach (song, songs)
    {
        Collection::add(song);
    }
}
