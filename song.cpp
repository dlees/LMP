#include "song.h"

#include "database.h"
#include "media_manager.h"

Song::Song(const QString &filename_)
    : Music_Item(filename_),
      filename(filename_),
      seconds(0),
      rating(3),
      created(QDateTime::currentDateTime()),
      is_playing(false)

{
    Database::get()->add_song(get_id(), filename, get_name(), created);
}

void Song::add_secs(int amt)
{
    seconds += amt;
}

void Song::add_child(QStandardItem *)
{
    return;
}

QLinkedList<Song*> Song::get_leaves()
{
    QLinkedList<Song*> list;
    list.push_back(this);
    return list;
}

void Song::begin_playing()
{
    Media_Manager::get()->play_new(this);
    is_playing = true;
}
