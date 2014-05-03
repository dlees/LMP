#include "library.h"

#include "collection.h"
#include "song.h"
#include "database.h"
#include "playlist.h"
#include "catalog.h"

#include "error.h"

#include <iostream>



Library::Library()
    : Collection("Library"),
      songs(new Playlist("All Songs"))
{
    add(songs);

// Load stuff from database
    try
    {
        load_songs();
        load_playlists();
    }
    catch (Error &msg)
    {
        msg.print_error_msg();
        throw msg;
    }
}

// load songs from the database
void Library::load_songs()
{
    Song *song;
    QList< SongInfo > *songsInfo;
    songsInfo = Database::get()->get_all_song_info();

    //we now have a list of lists of structs with relevant info
    SongInfo songI;
    foreach(songI, *songsInfo){
        song = new Song(songI.songName, songI.songID, songI.fileName, songI.seconds,
                        songI.rating, songI.created, songI.artistName, songI.albumName);

        add_song(song);

        if (songI.songID > Music_Item::max_id)
            Music_Item::max_id = songI.songID;
    }

    qDebug() << "Songs Loaded";
}

// load playlists from the database
void Library::load_playlists()
{
    Collection *playlist;
    QList<PlaylistInfo> *listsInfo;
    listsInfo = Database::get()->get_all_list_info();

    QList<Music_Item*> songList;
    int ID = 0;

    PlaylistInfo playlistI;
    foreach(playlistI, *listsInfo)
    {
        songList.clear();

        foreach(ID, playlistI.songIDs)
        {
            songList.append(get_item(ID));
        }

        if (playlistI.is_catalog)
        {
            playlist = new Catalog(playlistI.name, playlistI.ID, songList);
            add_catalog(static_cast<Catalog*>(playlist));
        }
        else //its a playlist
        {
            playlist = new Playlist(playlistI.name, playlistI.ID, songList);
            add_playlist(static_cast<Playlist*>(playlist));
        }

        if (playlistI.ID > Music_Item::max_id)
            Music_Item::max_id = playlistI.ID;
    }
    qDebug() << "Playlists Loaded";
}

Song *Library::get_song(const QString& filename)
{
    Song *new_song;
    QMap<QString, Song*>::Iterator i;

    // if the filename already exists
    if ((i = filename_to_song.find(filename)) != filename_to_song.end())
    {
        new_song = i.value();
    }
    else
    {   // make a new Song
        new_song = new Song(filename);
        add_song(new_song);
    }
    return new_song;
}

Catalog *Library::get_catalog(const QString &name)
{
    Catalog *catalog;
    QMap<QString, Catalog*>::Iterator i;

    // if the catalog already exists
    if ((i = name_to_catalog.find(name)) != name_to_catalog.end())
    {
        catalog = i.value();
    }
    else
    {   // make a new catalog
        catalog = new Catalog(name);
        add_catalog(catalog);
    }
    return catalog;
}

Music_Item *Library::get_item(int id)
{
    return id_to_item[id];
}

void Library::add_song(Song *song)
{
    songs->add(song);
    id_to_item.insert(song->get_id(), song);
    filename_to_song.insert(song->get_filename(), song);
}

void Library::add_playlist(Playlist *list)
{
    add(list);
//    playlists->add(list);

    id_to_item.insert(list->get_id(), list);

}

void Library::add_catalog(Catalog *list)
{
    add(list);
    id_to_item.insert(list->get_id(), list);

    name_to_catalog.insert(list->get_name(), list);
}

void Library::remove(int i)
{
    if (i == 0)
    {
        Error::print_error_msg_str("Can't delete \"All Songs\".");
        return;
    }

    Playlist* p = dynamic_cast<Playlist*>(get_children().at(i));

    if (!(p && delete_playlist(p)))
        return;

    Collection::remove(i);
}

bool Library::delete_playlist(Playlist *p)
{
    // if it is the current playlist, don't delete
    if (p->get_is_playing())
    {
        Error::print_error_msg_str("Can't delete " + p->get_name()
                                   + ".\nIt is being played.");
        return false;
    }

    id_to_item.remove(p->get_id());

    // remove from database here

    return true;
}


Playlist *Library::get_all_songs()
{ return dynamic_cast<Playlist*>(songs);}
