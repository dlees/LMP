#include "music_item.h"

int Music_Item::max_id = -1;

Music_Item::Music_Item(const QString &name_)
    : name(name_),
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
            << (get_name())
            << (total_secs())
               ;
}
