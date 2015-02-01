#include "time_ds.h"

#include <QTime>

std::string get_human_time(time_t value)
{
    QDateTime timestamp;
    timestamp.setTime_t(value);
    return timestamp.toString(Qt::SystemLocaleDate).toStdString();
}
