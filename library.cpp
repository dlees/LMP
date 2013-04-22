#include "library.h"

#include "collection.h"
#include "song.h"
#include "database.h"
#include "playlist.h"

#include "error.h"

#include <iostream>



Library::Library()
    : Collection("Library"),
      songs(new Playlist("All Songs")),
      playlists(new Collection("Playlists"))
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
    //QList< QList<QString> >::iterator it;
    songsInfo = Database::get()->get_all_song_info();

    //we now have a list of lists of strings of this info in this order:
    //ID, name, filename, created, seconds, album, artist
    //iterate over the giant list
    SongInfo songI;
    //QString temp;
    foreach(songI, *songsInfo){
        song = new Song(songI.songName, songI.songID, songI.fileName, songI.seconds,
                        0, songI.created, songI.artistName, songI.albumName);
        add_song(song);
    }

   /* for(it = songsInfo->begin(); it !=songsInfo->end(); ++it){
        datetime = QDate::fromString((*it).at(3));
        song = new Song((*it)[1], (*it)[0].toInt(), it[2], it[4].toInt(),
                        0, datetime, it[6], it[5]);
        add_song(song);
    }*/
    //new Song(const QString &name_, int id, const QString &filename_,
    //  int seconds_, int rating_, QDateTime created, const QString &artist_,
    //  const QString &album_
}

// load playlists from the database
void Library::load_playlists()
{
//    Playlist *playlist;

    //get from database somehow ...

//    add_playlist(playlist);
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
