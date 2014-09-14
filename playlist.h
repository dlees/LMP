#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "collection.h"


/*
    A playlist is a collection of only songs
    This is something to keep in mind.

*/

class Playlist : public Collection
{
public:
    Playlist(const QString &name = "Default Playlist");
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

    void move(int i, int pos_to_move_to);


private:
    int cur;
};

#endif // PLAYLIST_H
