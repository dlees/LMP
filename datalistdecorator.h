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
DataListDecorator *create_playlist_decorator(const std::string &name);
DataListDecorator *add_to_collection_decorator(Collection *);
DataListDecorator *filter_decorator(const std::string &name, int value);
DataListDecorator *sort_decorator(const std::string &name);

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
    FilterByValue(int value_) :
        value(value_)
    {}

    virtual DataList *decorate(DataList *datalist);
private:
    int value;
};

/*
 * Sort all data by value
 */
class SortByValue : public DataListDecorator {
public:
    virtual DataList *decorate(DataList *datalist);
};



#endif // DATALISTDECORATOR_H
