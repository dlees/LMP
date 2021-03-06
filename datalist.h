#ifndef DATALIST_H
#define DATALIST_H

#include <string>
#include <time.h>

#include <vector>
#include <map>

class DataPoint;

typedef std::vector<DataPoint*>::iterator datalist_iter_t;
typedef std::vector<DataPoint*>::const_iterator const_datalist_iter_t;


#include "datapointcomparers.h"

class DataListVisitor {
public:
    virtual void operator()(DataPoint*) = 0;
};

class DataList
{
public:
    static DataList *get_instance();
    static DataList *get_instance(const DataList &rhs);

    virtual void add_data_point(DataPoint *data) = 0;

    virtual void remove_data_point(int id) = 0;
    virtual void remove_data_point(const std::string &name) = 0;

    virtual void filter_id(const std::vector<int> &ids_to_include) = 0;

    // Will make it such that data only includes points with time:
    //    start_time <= point.get_time() <= end_time
    virtual void filter_time(time_t start_time, time_t end_time) = 0;

    // filters data such that it includes only values that satisfy
    //   comparitor(data.value, rhs_value) == true
    // Only makes sense if comparitor compares DataValues
    virtual void filter_value(int rhs_value, DataPointComparer *comparitor) = 0;

    virtual datalist_iter_t begin() { return data.begin();}
    virtual datalist_iter_t end() {return data.end();}
    virtual const_datalist_iter_t constBegin() const { return data.begin();}
    virtual const_datalist_iter_t constEnd() const {return data.end();}

    int size() {return data.size();}

    void perform_function(DataListVisitor &function);

    virtual ~DataList();

    std::vector<int> get_ids();
    std::map<int, int> get_values();

protected:
    DataList();

    std::vector<DataPoint*> data;
};


class SumDataList : public DataListVisitor {

public:
    SumDataList() : sum(0) {}

    virtual void operator()(DataPoint*);

    int sum;
};

#endif // DATALIST_H
