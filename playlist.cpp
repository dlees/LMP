#include "playlist.h"

#include "song.h"

Playlist::Playlist()
    : Collection("Unsaved Playlist"),
      cur(0)
{
}

Playlist::Playlist(Collection *col)
    : Collection(col->get_name()+"_playlist")
{
   Music_Item *item;
   foreach (item, col->get_children())
       add(item);


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

void Playlist::prev()
{
    get_children().at(cur = (cur+count()-1)%count())->begin_playing();
}

void Playlist::next()
{
    get_children().at(cur = (cur+1)%count())->begin_playing();
}
