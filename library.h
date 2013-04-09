#ifndef LIBRARY_H
#define LIBRARY_H

#include "collection.h"

class Song;
class Playlist;

class Library : public Collection
{
public:
    Library();

    void add_song(Song *);
//    void add_artist(Artist *);
//    void add_album(Album *);
    void add_playlist(Playlist *);

private:
    Collection *songs;
    Collection *playlists;
};

#endif // LIBRARY_H
