#include "datalist.h"

#include <Qt>
#include <vector>

#include "datapoint.h"
#include "datapointcomparers.h"

using namespace std;

// Deletes data and returns true if value satisfies:
//   compare(data.value, rhs_value) == true
class DataPointPredicateDeleter {
public:
    DataPointPredicateDeleter(DataPointComparer *comp, DataPoint *rhs) :
        comparer(comp), rhs_point(rhs) {}

    bool operator()(DataPoint *dp) {
        if (comparer->compare(dp, rhs_point))
        {
            delete dp;
            return true;
        }
        else
        {
            return false;
        }
    }
private:
    DataPointComparer *comparer;
    DataPoint *rhs_point;
};

class TimeSortedDataList : public DataList
{
public:

    void add_data_point(DataPoint *data_point) {
        data.push_back(data_point);
    }

    void remove_song(int id) {
        throw "Unimplemented";
    }

    void remove_song(const std::string &name) {
        throw "Unimplemented";
    }

    void filter_time(time_t start_time, time_t end_time) {
        DataPoint *point_to_compare =
                DataPoint::get_instance("",0, 0 ,end_time);

        // Need to remove twice, one for before start time, one for after end time


    }

    void filter_value(int rhs_value, DataPointComparer *comparitor)
    {
        DataPoint *point_to_compare =
                DataPoint::get_instance("", 0, DataValue::get_instance(rhs_value), 0);

        DataPointPredicateDeleter predicate(comparitor, point_to_compare);

        data.erase(remove_if(data.begin(), data.end(), predicate), data.end());

        delete point_to_compare;
    }

private:
};

DataList *DataList::get_instance() {return new TimeSortedDataList;}

DataList::DataList()
{
}