#include "dynamicplaylist.h"

#include "database.h"
#include "datalistdecorator.h"
#include <QTime>
#include "Error.h"

DynamicPlaylist::DynamicPlaylist()
{    
    DataList *datalist = Database::get()->get_sec_count_data();

    try {
    datalist = create_decorator_combiner()
            ->build(new FilterByTime(QDateTime::currentDateTime().addDays(-1).toTime_t(), QDateTime::currentDateTime().toTime_t()))
            ->build(new TotalUp())
            ->build(sort_decorator("value"))
            ->build(filter_decorator("less than", 100))
            ->build(create_playlist_decorator("Recently Played"))
        ->decorate(datalist);
    } catch (Error &e) {
        e.print_error_msg();
    }

    delete datalist;

}
