#include "datapoint.h"

#include "music_item.h"

class SimpleDataValue : public DataValue
{
public:
    SimpleDataValue(int value_)
        : value(value_)
    {}

    virtual int get_value() const {return value;}

private:
    int value;
};

class SecCountData : public DataValue
{
public:
    SecCountData(int start_pos_, int end_pos_)
        : start_pos(start_pos_), end_pos(end_pos_)
    {}

    virtual int get_value() const {return end_pos - start_pos;}

private:
    int start_pos;
    int end_pos;
};

DataPoint::DataPoint(const std::string &name_, int id_, DataValue *value_, time_t time_) :
    name(name_), id(id_), value(value_), time(time_)
{
}

DataValue *DataValue::get_instance(int value)
{
    return new SimpleDataValue(value);
}

DataValue *DataValue::get_instance(int secCount_start, int secCount_end)
{
    return new SecCountData(secCount_start, secCount_end);
}
