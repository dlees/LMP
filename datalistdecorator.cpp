#include "datalistdecorator.h"

#include <Qt>

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

DataListDecorator::DataListDecorator()
{
}

DataListDecorator *create_playlist_decorator(const std::string &name, bool is_catalog)
{
    if (is_catalog)
        return new CatalogCreatorDecorator(name);

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

#include "catalog.h"
DataList *PlaylistCreatorDecorator::decorate(DataList *datalist)
{
    vector<int> song_ids = datalist->get_ids();

    Media_Manager::get()->new_playlist(song_ids, name);

    return datalist;
}

DataList *CatalogCreatorDecorator::decorate(DataList *datalist)
{
    vector<int> song_ids = datalist->get_ids();

    QString Qname = QString::fromStdString(name);

    Media_Manager::get()->create_catalog(Qname, 0);

    foreach (int song_id, song_ids) {
        Music_Item * item = Media_Manager::get()->get_music_item(song_id);
        Media_Manager::get()->add_to_catalog(Qname, item);
    }

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
    collection->add_items(datalist->get_ids());
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
    std::map<int, int> IDToSum;

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;
        int ID = point->get_id();

        if (IDToSum.find(ID) == IDToSum.end())
            IDToSum[ID] = 0;

        int total = IDToSum[ID] + point->get_value()->get_value();

        IDToSum[ID] = total;
    }

    DataList *newList = DataList::get_instance();
    for (map<int,int>::iterator map_iter = IDToSum.begin() ;
         map_iter != IDToSum.end() ; ++map_iter)
    {
        newList->add_data_point(DataPoint::get_instance(
             "", map_iter->first, DataValue::get_instance(map_iter->second), 0));
    }
    delete datalist;
    return newList;
}

DataList *RunningTotal::decorate(DataList *datalist)
{
    std::map<int, int> IDToSum;

    DataList *newList = DataList::get_instance();

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;
        int ID = point->get_id();

        if (IDToSum.find(ID) == IDToSum.end())
            IDToSum[ID] = 0;

        int total = IDToSum[ID] + point->get_value()->get_value();

        IDToSum[ID] = total;

        newList->add_data_point(DataPoint::get_instance(
             point->get_name(), ID, DataValue::get_instance(total), point->get_time()));
    }

    delete datalist;
    return newList;
}

class PrintDataPoint : public DataListVisitor {
public:
    std::ostream &out;
    PrintDataPoint(std::ostream &out_) : out(out_) {}
    void operator ()(DataPoint* point) {
        out << point->to_string() << endl;
    }
};

DataList *LogDatalist::decorate(DataList *datalist)
{
    datalist->perform_function(PrintDataPoint(cout));
    return datalist;
}

DataList *OutputToFile::decorate(DataList *datalist)
{
    ofstream fout(filename);
    datalist->perform_function(PrintDataPoint(fout));
    return datalist;
}

DataList *InvertTime::decorate(DataList *datalist)
{
    DataList *newList = DataList::get_instance();

    time_t last_time_end = 0;

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;

        time_t this_time_start =   point->get_time() - point->get_value()->get_value()/1000;

        newList->add_data_point(DataPoint::get_instance(
           "name", 1, DataValue::get_instance(last_time_end, this_time_start), 0));

        last_time_end = point->get_time();
    }

    delete datalist;
    return newList;
}


class GetNameForDataPoint : public DataListVisitor {
public:
    void operator ()(DataPoint* point) {
        int ID = point->get_id();
        point->set_name(Media_Manager::get()->get_music_item(ID)->get_name().toStdString());
    }
};

DataList *GetNamesFromIDs::decorate(DataList *datalist)
{
    datalist->perform_function(GetNameForDataPoint());
    return datalist;
}
