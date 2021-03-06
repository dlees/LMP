#include "datapoint.h"

#include "music_item.h"
#include "time_ds.h"

using namespace std;

class SimpleDataValue : public DataValue
{
public:
    SimpleDataValue(DataValueResult value_)
        : value(value_)
    {}

    virtual DataValue *clone() const {return new SimpleDataValue(value);}

    virtual DataValueResult get_value() const {return value;}

private:
    DataValueResult value;
};

class TimeRangeValue : public DataValue
{
public:
    TimeRangeValue(time_t start_, time_t end_)
        : start(start_), end(end_)
    {}

    virtual DataValueResult get_value() const {return end - start;}

    virtual DataValue *clone() const {
        return new TimeRangeValue(start, end);
    }

    virtual string to_string() const {
        ostringstream stream;
        stream << get_human_time(start) << "\t" <<get_human_time(end)<< "\t" << get_value();
        return stream.str();
    }

private:
    time_t start;
    time_t end;
};

DataPoint::DataPoint(const std::string &name_, int id_, DataValue *value_, time_t time_) :
    name(name_), id(id_), value(value_), time(time_)
{
}

DataValue *DataValue::get_instance(DataValueResult value)
{
    return new SimpleDataValue(value);
}

DataValue *DataValue::get_instance(int secCount_start, int secCount_end)
{
    return new SecCountData(secCount_start, secCount_end);
}

DataValue *DataValue::get_instance(DataList *datalist)
{
    return new DataListDataValue(datalist);
}

DataValue *DataValue::get_instance(time_t start_time, time_t end_time)
{
    return new TimeRangeValue(start_time, end_time);
}

#include "datalist.h"
DataValueResult DataListDataValue::get_value() const
{
    SumDataList sumDataList;
    datalist->perform_function(sumDataList);
    return sumDataList.sum;
}

string DataListDataValue::to_string() const
{
    return std::to_string((long long)get_value());;
}

DataValue *DataListDataValue::clone() const
{
    return new DataListDataValue(DataList::get_instance(*datalist));
}

DataList *DataListDataValue::get_datalist()
{
    return datalist;
}
