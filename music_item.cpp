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

    //DOESN:T COME HERE ANYMORE
    /*
    switch (role)
    {
        case Qt::DisplayRole:
            return get_name();

        default:
            return QVariant();
    }
    */
    return QVariant();
}

QStringList Music_Item::get_headers() const
{
    return QStringList()
            << "Name"
            << "Seconds Played"
               ;
}

QList<QStandardItem*> Music_Item::get_column_data() const
{
    return QList<QStandardItem*>()
            << new QStandardItem(get_name())
            << new QStandardItem(QString::number(total_secs()))
               ;
}
