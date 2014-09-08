#include "datalistdecorator.h"

#include <Qt>

#include <string>
using namespace std;

DataListDecorator::DataListDecorator()
{
}

DataListDecorator *create_playlist_decorator(const std::string &name)
{
    return new PlaylistCreatorDecorator(name);
}


DataListDecoratorCombiner *create_decorator_combiner()
{
    return new DataListDecoratorCombiner;
}

DataListDecorator *add_to_collection_decorator(Collection *pl)
{
    return new AddToCollection(pl);
}


DataListDecorator *filter_decorator(const string &name, int value)
{
    return new FilterByValue(value);
}

DataListDecorator *sort_decorator(const string &name)
{
    return new SortByValue;
}


DataList *DataListDecoratorCombiner::decorate(DataList *datalist)
{
    foreach (DataListDecorator *decorator , decorators) {
        datalist = decorator->decorate(datalist);
    }
    return datalist;
}

DataListDecoratorCombiner *DataListDecoratorCombiner::build(DataListDecorator *dec)
{
    decorators.push_back(dec);
    return this;
}



#include "datalist.h"
#include "datapoint.h"
#include "media_manager.h"
vector<int> convert_to_song_ids(DataList *datalist)
{
    vector<int> song_ids;

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter)
    {
        song_ids.push_back((*iter)->get_id());
    }

    return song_ids;
}

DataList *PlaylistCreatorDecorator::decorate(DataList *datalist)
{
    vector<int> song_ids = convert_to_song_ids(datalist);

    Media_Manager::get()->new_playlist(song_ids, name);

    return datalist;
}

#include "playlist.h"
DataList *AddToCollection::decorate(DataList *datalist)
{
    vector<int> song_ids = convert_to_song_ids(datalist);

    foreach (int id, song_ids)
    {
        collection->add(Media_Manager::get()->get_music_item(id));
    }
    return datalist;
}

#include "datapointcomparers.h"
DataList *FilterByValue::decorate(DataList *datalist)
{
    DataPointComparer *comparer = get_DataPointComparer("value", "less than");
    datalist->filter_value(value, comparer);
    delete comparer;
    return datalist;
}

DataList *SortByValue::decorate(DataList *datalist)
{
    DataPointComparer *comparer = get_DataPointComparer("value", "greater than");
    sort(datalist->begin(), datalist->end(), DataPointComparerSTLAdapter(comparer));
    delete comparer;
    return datalist;
}

