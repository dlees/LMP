#include "song.h"

#include "database.h"


Song::Song(const QString &filename_, const QString &name)
    : Music_Item(name),
      filename(filename_),
      seconds(0),
      rating(3),
      created(QDateTime::currentDateTime())

{
    Database::get()->add_song(get_id(), filename, get_name(), created);
}


void Song::add_secs(int amt)
{
    seconds += amt;
}

QLinkedList<Song*> Song::get_leaves()
{
    QLinkedList<Song*> list;
    list.push_back(this);
    return list;
}
