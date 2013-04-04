#include "library.h"

#include "collection.h"
#include "song.h"

Library::Library()
    : Collection("Library"),
      songs(new Collection("All Songs"))
{
    add(songs);
}

void Library::add_song(Song *song)
{
    songs->add(song);
}
