#include "library.h"

#include "collection.h"
#include "song.h"
#include "database.h"
#include "playlist.h"

#include "error.h"

#include <iostream>



Library::Library()
    : Collection("Library"),
      songs(new Playlist("All Songs"))
{
    add(songs);
//    add(new Collection("Artists"));
//    add(new Collection("Albums"));
//    add(playlists);


// Load stuff from database
    load_songs();
    load_playlists();
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
                        0, songI.created, songI.artistName, songI.albumName);
        add_song(song);
    }
}

// load playlists from the database
void Library::load_playlists()
{
    Playlist *playlist;
    QList<PlaylistInfo> *listsInfo;
    listsInfo = Database::get()->get_all_list_info();
    QList<Music_Item*> songList;
    int ID = 0;

    PlaylistInfo playlistI;
    foreach(playlistI, *listsInfo){
        foreach(ID, playlistI.songIDs){
            songList.append(get_song(ID));
        }
        playlist = new Playlist(playlistI.name, playlistI.ID, songList);
        add_playlist(playlist);
    }
}

Song *Library::get_song(const QString& filename)
{
    // TODO: check to see if it already exists


    Song *new_song = new Song(filename);
    add_song(new_song);
    return new_song;
}

Song *Library::get_song(int id)
{
    return static_cast<Song*>(id_to_item[id]);

    //static_cast is ok because we know it is going to
    // be a song
}

void Library::add_song(Song *song)
{
    songs->add(song);
    id_to_item.insert(song->get_id(), song);
}

void Library::add_playlist(Playlist *list)
{
    add(list);
//    playlists->add(list);

    id_to_item.insert(list->get_id(), list);
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
        Error::print_error_msg_str("Can't delete " + p->get_name() + ".\nIt is being played.");
        return false;
    }

    id_to_item.remove(p->get_id());

    // remove from database here

    return true;
}
