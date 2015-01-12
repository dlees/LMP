#include "media_manager.h"

#include "song.h"
#include "catalog.h"
#include "library.h"
#include "playlist.h"
#include "collection.h"
#include "database.h"

Media_Manager::Media_Manager()
    : playing(Playing_Song::get())
{
    cur_list = lib.get_all_songs();
    cur_list->start_playing();

    connect(&playing, SIGNAL(became_paused()),
            this, SLOT(became_paused_slot()));
    connect(&playing, SIGNAL(started_playing()),
            this, SLOT(started_playing_slot()));
    connect(&playing, SIGNAL(AlmostDone_PlaySong()),
            this, SLOT(next()));

    qDebug() << "Media Manager Loaded" ;
}

void Media_Manager::play_cur()
{
    if (playing.is_paused())
        playing.play();
    else
        playing.pause();
}

void Media_Manager::start_up()
{
     center = cur_list;
}

void Media_Manager::play_new(QString filename)
{
    Song *new_song = lib.get_song(filename);

    play_new(new_song);
}

void Media_Manager::play_new(Song *song)
{    
    if (playing.get_cur_song() != 0 && playing.get_cur_song()->get_rating() > 5)
        change_rating_cur_song(5);

    playing.replace_song(song);
    playing.play();
}

void Media_Manager::first()
{
    cur_list->set_cur(-1);
    cur_list->next();
}

void Media_Manager::prev()
{
    cur_list->prev();
}

void Media_Manager::next()
{
    cur_list->next();
}

void Media_Manager::add_cur_to_playlist()
{
    center->add(playing.get_cur_song());
}

void Media_Manager::add_to_playlist(Song *song)
{
    cur_list->add(song);
}

void Media_Manager::switch_playlist(Collection *col)
{
    switch_playlist(new Playlist(col));
}

void Media_Manager::switch_playlist(Playlist *playlist, int pos)
{
    // tell the old list that it isn't currently playing anymore
    if (cur_list)
        cur_list->stop_playing();

    cur_list = playlist;
    cur_list->set_cur(pos);

    // tell the new one that it is playing
    cur_list->start_playing();

    emit playlist_changed(cur_list);

    // if we aren't playing a song
    //if (playing.is_paused())
        first(); // go to the first song in the playlist
}

void Media_Manager::new_playlist()
{
    lib.add_playlist(new Playlist("Default Playlist", Database::get()));
}

void Media_Manager::new_playlist(QStringList *filenames, const QString &name)
{
    Playlist *new_pl = new Playlist(name, Database::get());

    foreach (QString filename, *filenames)
    {
        new_pl->add(lib.get_song(filename));
    }

    add_playlist_to_library(new_pl);
}

void Media_Manager::add_playlist_to_library(Playlist *new_pl)
{
    lib.add_playlist(new_pl);

    set_center(new_pl);

    if (playing.is_paused())
        switch_playlist(new_pl);
}

void Media_Manager::new_playlist(const std::vector<int> &song_ids, const std::string &name)
{
    Playlist *new_pl = new Playlist(QString::fromStdString(name), 0);

    new_pl->add_items(song_ids);

    add_playlist_to_library(new_pl);
}


void Media_Manager::set_center(Collection *new_center)
{
    center = new_center;

    emit center_changed(new_center);
}

Collection *Media_Manager::create_catalog(const QString &name, Database *db)
{
    Catalog *catalog = new Catalog(name, db);
    lib.add_catalog(catalog);
    set_center(catalog);
    return catalog;
}

void Media_Manager::add_to_catalog(const QString &catalog_name, Music_Item *item)
{
    lib.get_catalog(catalog_name)->add(item);
}

void Media_Manager::change_rating_cur_song(int rating)
{
    Playing_Song::get().get_cur_song()->change_rating(rating);
}

Music_Item *Media_Manager::get_music_item(int id)
{
    return lib.get_item(id);
}

void Media_Manager::move_cur_song_up()
{
    cur_list->move_up(playing.get_cur_song());
}

Collection *Media_Manager::get_playlist()
{
    return cur_list;
}

Library *Media_Manager::get_library()
{
    return &lib;
}

Collection *Media_Manager::get_center()
{
    return center;
}
