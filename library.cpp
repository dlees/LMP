#include "library.h"

#include "collection.h"
#include "song.h"
#include "database.h"
#include "playlist.h"

#include "error.h"

Library::Library()
    : Collection("Library"),
      songs(new Collection("All Songs")),
      playlists(new Collection("Playlists"))
{
    add(songs);
//    add(new Collection("Artists"));
//    add(new Collection("Albums"));
//    add(playlists);
}

Song *Library::get_song(const QString& filename)
{
    // TODO: check to see if it already exists
    Song *new_song = new Song(filename);
    add_song(new_song);
    return new_song;
}

void Library::add_song(Song *song)
{
    songs->add(song);
}

void Library::add_playlist(Playlist *list)
{
    add(list);
//    playlists->add(list);
}

void Library::remove(int i)
{
    if (i == 0)
    {
        Error::print_error_msg_str("Can't delete \"All Songs\".");
        return;
    }

    Playlist* p = dynamic_cast<Playlist*>(get_children().at(i));
    // if it is the current playlist, don't delete
    if ( p && p->get_is_playing())
    {
        Error::print_error_msg_str("Can't delete " + p->get_name() + ".\nIt is being played.");
        return;
    }

    Collection::remove(i);
}

