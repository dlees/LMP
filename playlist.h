#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "collection.h"

class Playlist : public Collection
{
public:
    Playlist();
    Playlist(Collection *);
    Playlist(const QString &name, int id_,
             const QList<Music_Item*> &items);

    ~Playlist();

    void add(Music_Item *);

    virtual void select_child(int i);
    virtual void begin_playing();

    void next();
    void prev();

    void set_cur(int index);

private:
    int cur;
};

#endif // PLAYLIST_H
