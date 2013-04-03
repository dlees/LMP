#include "song.h"

#include "database.h"

int Song::max_id = -1;

Song::Song(QString filename_)
    : id(++max_id),
      filename(filename_),
      seconds(0),
      rating(3),
      created(QDateTime::currentDateTime())

{
    Database::get()->add_song(id, filename, created);
}


void Song::add_secs(int amt)
{
    seconds += amt;
}
