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

/*-
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
/*
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
/*
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
/*
 * Creates a new playlist using SongValueProxies with the datalist passed into it.
 */
class ProxiedPlaylistCreatorDecorator : public DataListDecorator {
public:
    ProxiedPlaylistCreatorDecorator(const std::string &name_) :
        name(name_)
    {}

    virtual DataList *decorate(DataList *datalist);
private:
    std::string name;
};

/*
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

/*
 * Removes all data with a value less than lowest_value
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


/*
 * Sort all data by value
 */
class SortByValue : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);
};

/*
 * Totals up values for each song
 * Makes time useless.
 */
class TotalUp : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);
};

class LogDatalist : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);
};





#endif // DATALISTDECORATOR_H
