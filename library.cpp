#include "library.h"

#include "collection.h"
#include "song.h"
#include "database.h"
#include "playlist.h"
#include "catalog.h"

#include "error.h"

#include <iostream>

#include "dynamicplaylist.h"

Library::Library()
    : Collection("Library", Database::get()),
      songs(new Playlist("All Songs", Database::get()))
{
    add(songs);
    id_to_item[songs->get_id()] = songs;

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
    QList<PlaylistInfo> *listsInfo;
    listsInfo = Database::get()->get_all_list_info();

    QList<int> item_ids;
    QMap<int, PlaylistInfo> all_playlists_info;
    foreach(PlaylistInfo info, *listsInfo)
    {
       item_ids.append(info.ID);
       all_playlists_info.insert(info.ID, info);
    }

    QList<Music_Item*> *items = get_items(item_ids, all_playlists_info);
    insert_items_no_db(*items);
    delete items;

    qDebug() << "Playlists Loaded";
}

/* This will return a list of all the music items for the item_ids passed in.
 * If an id for a collection doesn't exist, it will be created by the data in the map
 * REQUIRES: Songs to be loaded
 *
 * Remember to delete the list of items that this returns
**/
QList<Music_Item*> *Library::get_items(const QList<int> &item_ids, const QMap<int, PlaylistInfo> &all_playlists_info)
{
    QList<Music_Item*> *items = new QList<Music_Item*>;
    foreach(int id, item_ids)
    {
        // Check to see if this item already exists
        // This will pick up any songs or previously created collections
        if (id_to_item.contains(id))
        {
            items->push_back(id_to_item.find(id).value());
        }
        else // the item doesn't exist, let's create it
        {
            if (!all_playlists_info.contains(id))
            {
                qDebug() << "There is no info for id:" << id << " in the DB.";
                continue;
            }

            const PlaylistInfo &playlistI = all_playlists_info.value(id);
            if (playlistI.is_catalog)
            {
                items->push_back(load_catalog(playlistI, all_playlists_info));
            }
            else //its a playlist
            {
                items->push_back(load_playlist(playlistI));
            }

            if (playlistI.ID > Music_Item::max_id)
                Music_Item::max_id = playlistI.ID;
        }
    }
    return items;
}

Catalog *Library::load_catalog(const PlaylistInfo &catalog_info, const QMap<int, PlaylistInfo> &all_playlists_info)
{
    // doing this first without the items may prevent cycle problems if I
    // ever support them.
    Catalog *catalog = new Catalog(catalog_info.name, catalog_info.ID);

    // In this version, we aren't adding the catalog to the library
    // until all of the collections have been created.
    // in the future we should have the library pane show a catalog instead of the library.
    //add_catalog(catalog);
    id_to_item.insert(catalog->get_id(), catalog);
    name_to_catalog.insert(catalog->get_name(), catalog);

    // go load all playlists that the catalog depends on
    QList<int> item_ids;
    foreach(int ID, catalog_info.songIDs)
    {
       item_ids.append(ID);
    }
    QList<Music_Item*> *items = get_items(item_ids, all_playlists_info);

    catalog->insert_items_no_db(*items);
    delete items;
    return catalog;
}

Playlist *Library::load_playlist(const PlaylistInfo& playlistI)
{
    QList<Music_Item*> songList;
    foreach(int ID, playlistI.songIDs)
    {
        songList.append(get_item(ID));
    }

    Playlist *playlist = new Playlist(playlistI.name, playlistI.ID, songList);

    // see the comment in the load_catalog fxn
    //add_playlist(playlist);
    id_to_item.insert(playlist->get_id(), playlist);

    return playlist;
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
        catalog = new Catalog(name, Database::get());
        add_catalog(catalog);
    }
    return catalog;
}

Music_Item *Library::get_item(int id)
{
    QMap<int, Music_Item*>::Iterator iter = id_to_item.find(id);
    if (iter == id_to_item.end()) {
        throw Error(QString("Cannot find id %1 in database.").arg(id));
    }
    return *iter;
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
