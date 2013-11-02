#include "music_item.h"

#include "database.h"

int Music_Item::max_id = -1;

Music_Item::Music_Item(const QString &name_)
    : name(name_),
      is_playing(false),
      id(++max_id)
{

}

Music_Item::~Music_Item()
{

}

QStringList Music_Item::get_headers() const
{
    return QStringList()
            << "Name"
            << "Seconds Played"
               ;
}

QList<QVariant> Music_Item::get_column_data() const
{
    return QList<QVariant>()
            << (is_playing?"->":"") + get_name()
            << (total_secs()/1000)
               ;
}

void Music_Item::change_rating(int new_rating)
{
    Database::get()->save_rating_count(get_id(), new_rating);
    cur_rating = new_rating;
}
