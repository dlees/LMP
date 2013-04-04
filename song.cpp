#include "song.h"

#include "database.h"

int Song::max_id = -1;

Song::Song(const QString &filename_, const QString &name)
    : Music_Item(name),
      id(++max_id),
      filename(filename_),
      seconds(0),
      rating(3),
      created(QDateTime::currentDateTime())

{
    Database::get()->add_song(id, filename, get_name(), created);
}


void Song::add_secs(int amt)
{
    seconds += amt;
}
