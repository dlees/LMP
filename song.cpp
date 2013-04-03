#include "song.h"

int Song::max_id = -1;

Song::Song(QString filename_)
    : id(++max_id),
      filename(filename_),
      seconds(0),
      rating(3),
      created(QDateTime::currentDateTime())

{

}
