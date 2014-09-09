#ifndef DATALIST_H
#define DATALIST_H

#include <string>
#include <time.h>

#include <vector>

class DataPoint;

typedef std::vector<DataPoint*>::iterator datalist_iter_t;

#include "datapointcomparers.h"

class DataList
{
public:
    static DataList *get_instance();

    virtual void add_data_point(DataPoint *data) = 0;

    virtual void remove_song(int id) = 0;
    virtual void remove_song(const std::string &name) = 0;


    // Will make it such that data only includes points with time:
    //    start_time <= point.get_time() <= end_time
    virtual void filter_time(time_t start_time, time_t end_time) = 0;

    // filters data such that it includes only values that satisfy
    //   comparitor(data.value, rhs_value) == true
    // Only makes sense if comparitor compares DataValues
    virtual void filter_value(int rhs_value, DataPointComparer *comparitor) = 0;

    virtual datalist_iter_t begin() { return data.begin();}
    virtual datalist_iter_t end() {return data.end();}

    int size() {return data.size();}

    ~DataList(){
        foreach (DataPoint *datapoint, data){
            delete datapoint;
        }
    }

protected:
    DataList();

    std::vector<DataPoint*> data;
};


#endif // DATALIST_H