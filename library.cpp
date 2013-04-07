#include "library.h"

#include "collection.h"
#include "song.h"
#include "database.h"

Library::Library()
    : Collection("Library"),
      songs(new Collection("All Songs"))
{
    add(songs);
    add(new Collection("Artists"));
    add(new Collection("Albums"));
    add(new Collection("Playlists"));
}

void Library::add_song(Song *song)
{
    songs->add(song);
}
