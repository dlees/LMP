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


DataListDecorator *filter_decorator(const string &comp, int value)
{
    return new FilterByValue(comp, value);
}

DataListDecorator *sort_decorator(const string &name)
{
    return new SortByValue;
}

DataListDecorator *aggregator_decorator(const string &type)
{
    return new TotalUp;
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
/*
#include "songvalueproxy.h"
DataList *ProxiedPlaylistCreatorDecorator::decorate(DataList *datalist)
{
    Collection *col = new Playlist(QString::fromStdString(name));
    Media_Manager::get()->get_library()->add(col);

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        Music_Item *song = (Media_Manager::get()->get_music_item((*iter)->get_id()));

        qDebug() << "Adding Song" << song->get_name() << "to new playlist with " << song->get_rating() <<
                    ":" << (*iter)->get_value()->get_value() ;

        Music_Item *new_song = new SongValueProxy(song, 5, (*iter)->get_value()->get_value());

        col->add(new_song);
    }
    return datalist;
}*/

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
    DataPointComparer *comparer = get_DataPointComparer("value", comparison);
    datalist->filter_value(value, comparer);
    delete comparer;
    return datalist;
}
DataList *FilterByTime::decorate(DataList *datalist)
{
    datalist->filter_time(start_time, end_time);

    return datalist;
}


DataList *SortByValue::decorate(DataList *datalist)
{
    DataPointComparer *comparer = get_DataPointComparer("value", "greater than");
    sort(datalist->begin(), datalist->end(), DataPointComparerSTLAdapter(comparer));
    delete comparer;
    return datalist;
}

DataList *TotalUp::decorate(DataList *datalist)
{
    std::map<int, int> labelToSum;

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;
        int ID = point->get_id();

        if (labelToSum.find(ID) == labelToSum.end())
            labelToSum[ID] = 0;

        int total = labelToSum[ID];

        labelToSum[ID] = total + point->get_value()->get_value();
    }

    DataList *newList = DataList::get_instance();
    for (map<int,int>::iterator map_iter = labelToSum.begin() ;
         map_iter != labelToSum.end() ; ++map_iter)
    {
        newList->add_data_point(DataPoint::get_instance(
             "name", map_iter->first, DataValue::get_instance(map_iter->second), 0));
    }
    delete datalist;
    return newList;
}



DataList *LogDatalist::decorate(DataList *datalist)
{
    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;

        qDebug() <<
            "ID:" << point->get_id() <<
            "Name:" << QString::fromStdString(point->get_name()) <<
            "Value:" << point->get_value()->get_value() <<
            "Time:"<< point->get_time() ;
    }
    return datalist;
}


