#ifndef LIBRARY_H
#define LIBRARY_H

#include "collection.h"

#include "QMap"

class Music_Item;
class Song;
class Playlist;
class Catalog;

class Library : public Collection
{
public:
    Library();

    // loading things from database
    void load_songs();
    void load_playlists();

    // gets a song for a filename
    // creates it if it needs to
    Song *get_song(const QString &filename);

    Catalog *get_catalog(const QString &name);

    Music_Item *get_item(int id);

    virtual void remove(int);

    void add_song(Song *);
//    void add_artist(Artist *);
//    void add_album(Album *);
    void add_playlist(Playlist *);
    void add_catalog(Catalog *);

    // doesn't remove from this
    // returns true if it has been deleted
    bool delete_playlist(Playlist *);

    Playlist *get_all_songs();

private:
    Collection *songs;
    Collection *playlists;

    QMap<int, Music_Item*> id_to_item;
    QMap<QString, Song*> filename_to_song;
    QMap<QString, Catalog*> name_to_catalog;
};

#endif // LIBRARY_H
