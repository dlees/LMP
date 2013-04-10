#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "collection.h"

class Playlist : public Collection
{
public:
    Playlist();
    Playlist(Collection *);

    void add(Music_Item *);

    void next();
    void prev();

    void set_cur(int index);

private:
    int cur;
};

#endif // PLAYLIST_H
