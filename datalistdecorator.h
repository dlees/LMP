#ifndef DATALISTDECORATOR_H
#define DATALISTDECORATOR_H

#include <string>
#include <vector>

class DataList;

class DataListDecorator
{
public:
    virtual DataList *decorate(DataList *datalist) = 0;

protected:
    DataListDecorator();
};

/**
 * Allows you to create a list of decorators
 *
 * Iterates through the decorators in order
 **/
class DataListDecoratorCombiner : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);

    DataListDecoratorCombiner *build(DataListDecorator *dec);

private:
    std::vector<DataListDecorator*> decorators;
};

class Collection;
//factory_method
DataListDecoratorCombiner *create_decorator_combiner();
DataListDecorator *create_playlist_decorator(const std::string &name, bool is_catalog = false);
DataListDecorator *add_to_collection_decorator(Collection *);
DataListDecorator *filter_decorator(const std::string &comp, int value);
DataListDecorator *sort_decorator(const std::string &name);
DataListDecorator *aggregator_decorator(const std::string &type);
/**
 * Creates a new playlist out of the datalist passed into it.
 */
class PlaylistCreatorDecorator : public DataListDecorator {
public:
    PlaylistCreatorDecorator(const std::string &name_) :
        name(name_)
    {}

    virtual DataList *decorate(DataList *datalist);
private:
    std::string name;
};
/**
 * Creates a new catalog out of the datalist passed into it.
 */
class CatalogCreatorDecorator : public DataListDecorator {
public:
    CatalogCreatorDecorator(const std::string &name_) :
        name(name_)
    {}

    virtual DataList *decorate(DataList *datalist);
private:
    std::string name;
};

/**
 * Adds songs in datalist to playlist.
 */
class AddToCollection : public DataListDecorator {
public:
    AddToCollection(Collection *col) :
        collection(col)
    {}

    virtual DataList *decorate(DataList *datalist);
private:
    Collection *collection;
};

/**
 * Removes all data with a value comp than lowest_value
 */
class FilterByValue : public DataListDecorator {
public:
    FilterByValue(const std::string &comp, int value_) :
        comparison(comp), value(value_)
    {}

    virtual DataList *decorate(DataList *datalist);
private:
    const std::string &comparison;
    int value;
};

#include "datalist.h"

/**
 * Removes any data whose ID isn't in datalist_to_include
 */
class FilterByID : public DataListDecorator {
public:
    FilterByID(DataList *datalist_to_include) :
        ids_to_include(datalist_to_include->get_ids())
    {
    }

    FilterByID(std::vector<int> &ids_to_include_) :
        ids_to_include(ids_to_include_)
    {}

    FilterByID(int ID) :
        ids_to_include(1)
    {
        ids_to_include.push_back(ID);
    }

    virtual DataList *decorate(DataList *datalist);
private:
    std::vector<int> ids_to_include;
};

class FilterByTime : public DataListDecorator {
public:
    FilterByTime(time_t start, time_t end) :
        start_time(start), end_time(end)
    {}

    virtual DataList *decorate(DataList *datalist);
private:
    time_t start_time;
    time_t end_time;
};

/**
 * returns a datalist that includes the time ranges not in the original
 */
class InvertTime : public DataListDecorator {
public:
    InvertTime()
    {}

    virtual DataList *decorate(DataList *datalist);
};



/**
 * Sort all data by value
 */
class SortByValue : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);
};

/**
 * Populate the name field with the appropriate name for the id
 */
class GetNamesFromIDs : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);
};

/**
 * Totals up values for each unique ID
 * Makes time and name useless.
 */
class TotalUp : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);
};

/**
 * Replaces the value with the running total of the value
 * for each unique ID.
 * Preserves Time
 */
class RunningTotal : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);
};

/**
 *
 *  Input: Requires SecCount Values
 *      Contains Song IDs
 *  Granularity - Should we look at each millisecond? decisecond? second? minute?
 *      - this is the number of millisecond granularity to calculate to.
 *      - 1000 means a position is returned for every second
 *
 *  Result: Time = position in the song
 *      Value = number of times the position was played
 *      ID/Name = ID/Name of the Song
 *
 */
class PlayCountCalculator : public DataListDecorator {
public:
    PlayCountCalculator(int granularity_ms_ = 1000) :
        granularity_ms(granularity_ms_) {}

    virtual DataList *decorate(DataList *datalist);

private:
    int granularity_ms;
};

class LogDatalist : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);
};

class ExportToExcel : public DataListDecorator {
public:
    ExportToExcel(const std::string &filename_) :
        filename(filename_)
    {}

    virtual DataList *decorate(DataList *datalist);
private:
    const std::string filename;
};

#include "datapoint.h"

/**
 * Requires a DataList of DataListDataValues
 *
 *  Excel format:
 *
 *  Time | datalist1.name | datalist2.name |...
 *  Time1|datalist1.value1|datalist2.value1|...
 *  Time2|datalist1.value2|datalist2.value2|...
 *
 */
class ExportToExcelSplit : public DataListDecorator {
public:
    ExportToExcelSplit(const std::string &filename_) :
        filename(filename_)
    {}

    virtual DataList *decorate(DataList *datalist);
private:
    const std::string filename;

    void outputToFile(const std::map<time_t, std::vector<DataValueResult> > &spreadsheet, const std::vector<std::string> &headings);
};


class OutputToFile : public DataListDecorator {
public:
    OutputToFile(const std::string &filename_) :
        filename(filename_)
    {}

    virtual DataList *decorate(DataList *datalist);
private:
    const std::string filename;
};

/**
 * Split a datalist into a datalist of datalists
 * Each ID goes into a separate datalist.
 */
class SplitByID : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);
};

/**
 * Split a datalist into a datalist of datalists
 * Every delta_seconds_ seconds goes into a separate datalist
 */
class SplitByTime : public DataListDecorator {
public:
    SplitByTime(int delta_seconds_) : delta_seconds(delta_seconds_) {}

    virtual DataList *decorate(DataList *datalist);
private:
    int delta_seconds;
};

/**
 * Merge all the data into the same ID
 */
class MergeIDToOne : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);
};

/**
 * Performs the decorator passed in on all datalists inside the datalist
 *
 *  Requires: datalist datavalues to be DataListDataValues
 *
 */
class SplitDatalistDecorator : public DataListDecorator {
public:
    SplitDatalistDecorator(DataListDecorator *decorator_) :
        decorator(decorator_) {}

    virtual DataList *decorate(DataList *datalist);
private:
    DataListDecorator *decorator;
};


#endif // DATALISTDECORATOR_H
