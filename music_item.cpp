#include "music_item.h"

#include "database.h"

int Music_Item::max_id = -1;

Music_Item::Music_Item(const QString &name_)
    : name(name_),
      is_playing(false),
      id(++max_id),
      star_rating(2, 6)
{

}

Music_Item::Music_Item(const QString &name_, int id_, int rating)
    : name(name_), is_playing(false),
      id(id_), star_rating(rating, 6){}

Music_Item::~Music_Item()
{

}

QStringList Music_Item::get_headers() const
{
    return QStringList()
            << "Name"
            << "Seconds"
            << "Rating"
    ;
}

QList<QVariant> Music_Item::get_column_data() const
{
    return QList<QVariant>()
            << (is_playing?"->":"") + get_name()
            << (total_millisecs()/1000)
           // << qVariantFromValue(star_rating)
               ;
}

void Music_Item::change_rating(int new_rating)
{
    Database::get()->save_rating_count(get_id(), new_rating);
    star_rating.setStarCount(new_rating);
    emit_data_changed();
}

Star_Rating Music_Item::get_star_rating() const
{
    return star_rating;
}

