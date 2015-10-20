#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <string>
#include <sstream>
#include <time.h>
#include "time_ds.h"

class DataList;
class DataValue {
public:
    static DataValue *get_instance(int value);
    static DataValue *get_instance(time_t start_time, time_t end_time);
    static DataValue *get_instance(int secCount_start, int secCount_end);
    static DataValue *get_instance(DataList* datalist);

    virtual ~DataValue() {}

    virtual int get_value() const = 0;

    virtual std::string to_string() const {
        std::ostringstream stream;
        stream << get_value();
        return stream.str();
    }

    virtual DataValue *clone() const = 0;

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

    static DataPoint *get_instance(const DataPoint &point)
    {return new DataPoint(point);}

    DataPoint(const DataPoint& rhs)
    {
        name = rhs.get_name();
        id = rhs.get_id();
        value = rhs.get_value()->clone();
        time = rhs.get_time();
    }

    const std::string &get_name() const {return name;}
    int get_id() const {return id;}
    DataValue *get_value() const {return value;}
    const time_t get_time() const {return time;}

    void set_name(const std::string &name_) {name = name_;}
    void set_value(DataValue *value_) {value = value_;}

    const std::string to_string() {
        std::ostringstream stream;
        stream << get_id() <<
            "\t" << get_name() <<
            "\t " << get_value()->to_string() <<
            "\t "<< get_human_time(get_time());

        return stream.str();
    }

    virtual ~DataPoint() {
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

class SecCountData : public DataValue
{
public:
    SecCountData(int start_pos_, int end_pos_)
        : start_pos(start_pos_), end_pos(end_pos_)
    {}

    virtual int get_value() const {return end_pos - start_pos;}

    virtual DataValue *clone() const {return new SecCountData(start_pos, end_pos);}

    virtual std::string to_string() const {
        std::ostringstream stream;
        stream << start_pos << "\t" << end_pos << "\t" << get_value();
        return stream.str();
    }

    int get_start_pos() const {return start_pos;}
    int get_end_pos() const {return end_pos;}

private:
    int start_pos;
    int end_pos;
};

class DataListDataValue : public DataValue
{
public:
    DataListDataValue(DataList *datalist_) : datalist(datalist_) {}

    virtual int get_value() const;

    virtual std::string to_string() const;

    virtual ~DataListDataValue() {delete datalist;}

    virtual DataValue *clone() const;

    DataList *get_datalist();

private:
    DataList *datalist;
};

#endif // DATAPOINT_H
