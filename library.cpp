#include "library.h"

#include "collection.h"
#include "song.h"
#include "database.h"
#include "playlist.h"

Library::Library()
    : Collection("Library"),
      songs(new Collection("All Songs")),
      playlists(new Collection("Playlists"))
{
    add(songs);
    add(new Collection("Artists"));
    add(new Collection("Albums"));
    add(playlists);
}

void Library::add_song(Song *song)
{
    songs->add(song);
}

void Library::add_playlist(Playlist *list)
{
    playlists->add(list);
}
