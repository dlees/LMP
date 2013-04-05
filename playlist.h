#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "collection.h"

class Playlist : public Collection
{
public:
    Playlist();

    void add(Music_Item *);

    void set_cur(int i);
};

#endif // PLAYLIST_H
