#include "datapointcomparers.h"

#include "datapoint.h"
#include "error.h"

class TimeComparer : public DataPointComparer {
public:
    TimeComparer(const std::string & comp_type) : comp(get_comparitor<time_t>(comp_type)){}

    virtual bool compare(const DataPoint* lhs, const DataPoint* rhs) {
        return comp->compare(lhs->get_time(), rhs->get_time());
    }

private:
    Comparitor<time_t> *comp;
};

class ValueComparer : public DataPointComparer {
public:
    ValueComparer(const std::string & comp_type) : comp(get_comparitor<int>(comp_type)){}

    virtual bool compare(const DataPoint* lhs, const DataPoint* rhs) {
        return comp->compare(lhs->get_value()->get_value(), rhs->get_value()->get_value());
    }

private:
    Comparitor<int> *comp;
};

class IDComparer : public DataPointComparer {
public:
    IDComparer(const std::string & comp_type) : comp(get_comparitor<int>(comp_type)){}

    virtual bool compare(const DataPoint* lhs, const DataPoint* rhs) {
        return comp->compare(lhs->get_id(), rhs->get_id());
    }

private:
    Comparitor<int> *comp;
};

DataPointComparer *get_DataPointComparer(const std::string &parameter_name, const std::string &comp_type)
{
    if (parameter_name == "time") {
        return new TimeComparer(comp_type);
    }
    else if (parameter_name == "value") {
        return new ValueComparer(comp_type);
    }
    else if (parameter_name == "id") {
        return new IDComparer(comp_type);
    }
    else {
        throw Error(QString("DataPointComparer not implemented: ") + QString::fromStdString(parameter_name));
    }
}
