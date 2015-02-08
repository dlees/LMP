#include "datalist.h"

#include <Qt>
#include <vector>
#include <set>

#include "datapoint.h"
#include "datapointcomparers.h"

using namespace std;

// Deletes data and returns true if value satisfies:
//   compare(data.value, rhs_value) == true
class DeleteIfTrue {
public:
    DeleteIfTrue(DataPointComparer *comp, DataPoint *rhs) :
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

typedef std::set<DataPoint *, DataPointComparerSTLAdapter> DataPointSet;

// returns true and deletes if data isn't in the rhs list
class DeleteIfNotFound {
public:
    DeleteIfNotFound(DataPointSet &rhs) :
        rhs_points(rhs) {}

    bool operator()(DataPoint *dp) {
        if (rhs_points.find(dp) != rhs_points.end())
            return false;

        delete dp;
        return true;
    }
private:
    const DataPointSet &rhs_points;
};

class TimeSortedDataList : public DataList
{
public:

    void add_data_point(DataPoint *data_point) {
        data.push_back(data_point);
    }

    void remove_data_point(int id) {
        throw "Unimplemented";
    }

    void remove_data_point(const std::string &name) {
        throw "Unimplemented";
    }

    void filter_time(time_t start_time, time_t end_time) {
        DataPoint *end_time_point =
                DataPoint::get_instance("",0, 0 ,end_time);
        DataPoint *start_time_point =
                DataPoint::get_instance("",0, 0 ,start_time);

        // Need to remove twice, one for before start time, one for after end time
        DataPointComparer *less_than_time = get_DataPointComparer("time", "less than");
        DataPointComparer *greater_than_time = get_DataPointComparer("time", "greater than");

        qDebug() << "filter time:" << start_time << "-" << end_time;

        DeleteIfTrue predicate(less_than_time, start_time_point);
        DeleteIfTrue predicate2(greater_than_time, end_time_point);

        data.erase(remove_if(data.begin(), data.end(), predicate), data.end());
        data.erase(remove_if(data.begin(), data.end(), predicate2), data.end());

        delete less_than_time;
        delete greater_than_time;
        delete end_time_point;
        delete start_time_point;

    }

    void filter_id(const vector<int> &ids)
    {
        DataPointComparer *less_than_id = get_DataPointComparer("id", "less than");
        DataPointComparerSTLAdapter less_than_stl(less_than_id);

        DataPointSet points_to_compare(less_than_stl);
        foreach (int id, ids) {
            points_to_compare.insert(DataPoint::get_instance("", id, 0, 0));
        }

        DeleteIfNotFound predicate(points_to_compare);

        data.erase(remove_if(data.begin(), data.end(), predicate), data.end());

        foreach (auto point, points_to_compare) {
            delete point;
        }
        delete less_than_id;
    }

    void filter_value(int rhs_value, DataPointComparer *comparitor)
    {
        DataPoint *point_to_compare =
                DataPoint::get_instance("", 0, DataValue::get_instance(rhs_value), 0);

        DeleteIfTrue predicate(comparitor, point_to_compare);

        data.erase(remove_if(data.begin(), data.end(), predicate), data.end());

        delete point_to_compare;
    }

private:
};

DataList *DataList::get_instance() {return new TimeSortedDataList;}

void DataList::perform_function(DataListVisitor &function)
{
    for (datalist_iter_t iter = this->begin() ;
         iter != this->end() ; ++iter) {
        DataPoint *point = *iter;

        function(point);
    }
}


DataList::~DataList(){
    foreach (DataPoint *datapoint, data){
        delete datapoint;
    }
}

std::vector<int> DataList::get_ids()
{
    vector<int> song_ids;

    for (datalist_iter_t iter = this->begin() ;
         iter != this->end() ; ++iter)
    {
        song_ids.push_back((*iter)->get_id());
    }

    return song_ids;
}

std::map<int, int> DataList::get_values()
{
    map<int,int> id_to_value;

    for (datalist_iter_t iter = this->begin() ;
         iter != this->end() ; ++iter)
    {
        id_to_value[(*iter)->get_id()] = (*iter)->get_value()->get_value();
    }

    return id_to_value;
}

DataList::DataList()
{
}
