#include "dynamicplaylist.h"

#include "database.h"
#include "datalistdecorator.h"
#include <QTime>
#include "Error.h"

#include "datalist.h"
#include <vector>

using namespace std;

DynamicPlaylist::DynamicPlaylist() : Playlist("Recently Played", 0)
{    
    DataList *datalist = Database::get()->get_sec_count_data();

    try {
    datalist = create_decorator_combiner()
            ->build(new FilterByTime(QDateTime::currentDateTime().addDays(-1).toTime_t(), QDateTime::currentDateTime().toTime_t()))
            ->build(new TotalUp())
            ->build(sort_decorator("value"))
            ->build(filter_decorator("less than", 100*1000))
        ->decorate(datalist);
    } catch (Error &e) {
        e.print_error_msg();
    }

    add_items(datalist->get_ids());
    id_to_values = datalist->get_values();

    delete datalist;

}

QStringList DynamicPlaylist::get_headers() const
{
    return QStringList()
            << "Name"
            << "Sec/day"
            << "Seconds"
            << "Rating"
               ;
}

QList<QVariant> DynamicPlaylist::get_overriden_column_data(Music_Item *item) const {

    return QList<QVariant>()
            << (item->get_is_playing()?"->":"") + item->get_name()
            << id_to_values.at(item->get_id())/1000
            << (item->get_milliseconds()/1000)
            << qVariantFromValue(item->get_star_rating())
            ;
}

QVariant DynamicPlaylist::data(const QModelIndex &index, int role) const
{
    if (index.row() >= rowCount() || index.row() < 0)
        { return QVariant(); }

    Music_Item *element = get_children().at(index.row());

    QList<QVariant> columns = get_overriden_column_data(element);
    switch (role)
    {
        case Qt::DisplayRole:

            return columns.at(index.column());

        default:
            return QVariant();
    }

}
