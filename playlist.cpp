#include "playlist.h"

#include "song.h"
#include "error.h"

Playlist::Playlist()
    : Collection("New Playlist"),
      cur(0)
{
}

Playlist::Playlist(Collection *col)
    : Collection(col->get_name()+"_playlist"),
      cur(0)
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
    cur = Collection::count() - 1;
}

void Playlist::select_child(int i)
{
    Collection::select_child(i);
    cur = i;
}

void Playlist::prev()
{
    if (count() != 0)
    {
        get_children().at(cur = (cur+count()-1)%count())->begin_playing();
    }
    else
    {
        Error *error = new Error("No songs yet! Add songs to this playlist!");
        error->print_error_msg();
    }
}

void Playlist::next()
{
    if (count() != 0)
    {
        get_children().at(cur = (cur+1)%count())->begin_playing();
    }
    else
    {
        Error *error = new Error("No songs yet! Add songs to this playlist!");
        error->print_error_msg();
    }
}

void Playlist::set_cur(int index)
{
    cur = index;
}

Playlist::~Playlist()
{

}
