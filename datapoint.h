#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <string>
#include <time.h>

class DataValue {
public:
    static DataValue *get_instance(int value);
    static DataValue *get_instance(int secCount_start, int secCount_end);

    virtual int get_value() const = 0;

protected:
    DataValue() {}
};

class Music_Item;


class DataPoint
{
public:
    static DataPoint *get_instance(const std::string &name_,
            int id_, DataValue *value_, time_t time_)
    {return new DataPoint(name_, id_, value_, time_);}

    const std::string &get_name() const {return name;}
    int get_id() const {return id;}
    const DataValue *get_value() const {return value;}
    time_t get_time() const {return time;}

    ~DataPoint() {
        delete value;
    }

private:
    DataPoint(
            const std::string &name_,
            int id_,
            DataValue *value_,
            time_t time_);

    std::string name;
    int id;
    DataValue *value;
    time_t time;
};

#endif // DATAPOINT_H