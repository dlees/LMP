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

QVariant Music_Item::data(int role) const
{
    switch (role)
    {
        case Qt::DisplayRole:
            qDebug() << get_name();
            return get_name();

        default:
            return QVariant();
    }
}
