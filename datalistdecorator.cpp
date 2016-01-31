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
    std::map<int, DataPoint*> idToPoint;

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;
        int ID = point->get_id();

        if (IDToSum.find(ID) == IDToSum.end())
            IDToSum[ID] = 0;

        int total = IDToSum[ID] + point->get_value()->get_value();

        IDToSum[ID] = total;

        if (idToPoint.find(ID) == idToPoint.end()) {
            idToPoint[ID] = point;
        }
    }

    DataList *newList = DataList::get_instance();
    for (map<int,int>::iterator map_iter = IDToSum.begin() ;
         map_iter != IDToSum.end() ; ++map_iter)
    {
        newList->add_data_point(DataPoint::get_instance(
                                    idToPoint[map_iter->first]->get_name(), map_iter->first, DataValue::get_instance(map_iter->second), 0));
    }
    delete datalist;
    return newList;
}

DataList *RunningTotal::decorate(DataList *datalist)
{
    std::map<int, DataValueResult> IDToSum;

    DataList *newList = DataList::get_instance();

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;
        int ID = point->get_id();

        if (IDToSum.find(ID) == IDToSum.end())
            IDToSum[ID] = 0;

        DataValueResult total = IDToSum[ID] + point->get_value()->get_value();

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

std::string get_name(int ID)
{
    return Media_Manager::get()->get_music_item(ID)->get_name().toStdString();
}

class GetNameForDataPoint : public DataListVisitor {
public:

    void operator ()(DataPoint* point) {
        int ID = point->get_id();
        point->set_name(get_name(ID));
    }
};

DataList *GetNamesFromIDs::decorate(DataList *datalist)
{
    datalist->perform_function(GetNameForDataPoint());
    return datalist;
}


DataList *ExportToExcel::decorate(DataList *datalist)
{
    vector<vector<int> > sumOverTime;
    vector<int> ids;
    map<int, int> idToIndex;

    vector<time_t> times;

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;

        times.push_back(point->get_time());

        for (size_t i = 0 ; i < sumOverTime.size() ; i++) {
            sumOverTime.at(i).push_back(sumOverTime.at(i).at(times.size()-2));
        }

        if (idToIndex.find(point->get_id()) == idToIndex.end() ) {
            ids.push_back(point->get_id());
            idToIndex[point->get_id()] = sumOverTime.size();

            sumOverTime.push_back(vector<int>());
            for (size_t i = 0 ; i < times.size() ; i++) {
                sumOverTime[sumOverTime.size()-1].push_back(0);
            }
        }

        sumOverTime.at(idToIndex[point->get_id()]).at(times.size()-1) = point->get_value()->get_value();

    }

    ofstream fout(filename);

    fout << "Time\t" ;

    foreach (auto id , ids) {
        fout << get_name(id) << "\t";
    }

    fout << endl;

    for (size_t i = 0 ; i < times.size() ; i++) {
        fout << get_human_time(times[i]) << "\t";
        foreach (vector<int> itemHistory  , sumOverTime) {
            fout << itemHistory[i] << "\t";
        }
        fout << endl;
    }

    fout.close();
    return datalist;
}

void initializeRow(map<time_t, vector<DataValueResult> > &spreadsheet, time_t time, int size)
{
    spreadsheet[time] = vector<DataValueResult>(size, 0);
}

void insertColumn(map<time_t, vector<DataValueResult> > &spreadsheet, DataList *datalist, int indexOfList, int maxRowSize){

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;
        time_t dataTime = point->get_time();

        if (spreadsheet.find(dataTime) == spreadsheet.end()) {
            initializeRow(spreadsheet, dataTime, maxRowSize);
        }

        spreadsheet[dataTime].at(indexOfList) = point->get_value()->get_value();
    }
}

DataList *ExportToExcelSplit::decorate(DataList *datalist)
{
    map<time_t, vector<DataValueResult> > spreadsheet;
    vector<string> headings;

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;

        DataList *innerDataList = dynamic_cast<DataListDataValue*>(point->get_value())->get_datalist();

        if (!innerDataList) {
            throw new Error("SplitDatalistDecorator MUST take in a datalist that contains DataListDataValue");
        }

        insertColumn(spreadsheet, innerDataList, headings.size(), datalist->size());
        headings.push_back(point->get_name());
    }

    outputToFile(spreadsheet, headings);

    return datalist;
}

void ExportToExcelSplit::outputToFile(
        const map<time_t, vector<DataValueResult> > &spreadsheet,
        const vector<string> &headings)
{
    ofstream fout(filename);

    fout << "Time\t" ;

    foreach (string heading , headings) {
        fout << heading << "\t";
    }

    fout << endl;

    for (map<time_t, vector<DataValueResult> >::const_iterator iter = spreadsheet.begin() ;
            iter != spreadsheet.end() ; ++iter) {

        fout << get_human_time(iter->first) << "\t";

        foreach (DataValueResult value, iter->second) {
            fout << value << "\t";
        }

        fout << endl;
    }

    fout.close();
}

DataList *FilterByID::decorate(DataList *datalist)
{
    datalist->filter_id( ids_to_include);
    return datalist;
}

DataList* convert_to_datalist(vector<int> &list, string name, int id, int start = 0, int end = -1) {
    DataList *dataList = DataList::get_instance();

    if (end == -1) {
        end = list.size();
    }

    for (int i = start; i < end ; i++) {
        dataList->add_data_point(DataPoint::get_instance(name, id, DataValue::get_instance(list.at(i)), i));
    }
    return dataList;
}

int infer_end_of_song(vector<int> &list) {
    int num_consecutive_zeros = 0;

    // 100th of the array size - i.e. 5 seconds
    int num_zeros_to_infer_end = list.size() / 50;

    for (size_t i = 0; i < list.size() ; i++) {
        if (list.at(i) == 0) {
            num_consecutive_zeros++;
        } else {
            num_consecutive_zeros = 0;
        }

        if (num_consecutive_zeros >= num_zeros_to_infer_end) {
            return i;
        }
    }
    return list.size();
}

DataList *PlayCountCalculator::decorate(DataList *datalist)
{
    // Initial size is 1000 seconds because a song won't be longer than 1000 sec (I'm sure there's one)
    vector<int> songPositions(1000*1000/granularity_ms, 0);

    string name = "";
    int id = 0;

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;
        name = point->get_name();
        id = point->get_id();

        const SecCountData *secCount = dynamic_cast<const SecCountData*>(point->get_value());

        if (!secCount) {
            throw new Error("PlayCountCalculator MUST take in a datalist that contains SecCountData");
        }

        int intervalStart = secCount->get_start_pos()/granularity_ms;
        int intervalEnd = secCount->get_end_pos()/granularity_ms;

        for (int i = intervalStart; i < intervalEnd ; i++) {
            songPositions.at(i)++;
        }
    }

    delete datalist;
    DataList *payCountByPos = convert_to_datalist(songPositions, name, id, 0, infer_end_of_song(songPositions));
    return payCountByPos;
}

DataList *SplitByID::decorate(DataList *datalist)
{
    DataList *splitDatalist = DataList::get_instance();

    map<int, DataList*> idToDataList;

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;

        int cur_id = point->get_id();

        if (idToDataList.find(cur_id) == idToDataList.end()) {
            DataList *newDatalist = DataList::get_instance();
            splitDatalist->add_data_point(DataPoint::get_instance(point->get_name(), cur_id,
                                                            new DataListDataValue(newDatalist), point->get_time()));
            idToDataList[cur_id] = newDatalist;
        }

        idToDataList[cur_id]->add_data_point(DataPoint::get_instance(*point));
    }

    delete datalist;
    return splitDatalist;
}

DataList *SplitByTime::decorate(DataList *datalist)
{
    DataList *splitDatalist = DataList::get_instance();

    // The time_t is the start_time of the bucket
    map<time_t, DataList*> bucketToDataList;

    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;

        // In this implementation, sec_count data ignores the fact that
        // the time the song started playing could be before the start_time
        // of the bucket it will go in.
        // For example, delta = 1 hr. sec_count = 4:58-5:02.
        // This gets put into the 5:00 bucket.
        time_t cur_bucket = point->get_time() - point->get_time()%delta_seconds;

        if (bucketToDataList.find(cur_bucket) == bucketToDataList.end()) {
            DataList *newDatalist = DataList::get_instance();
            splitDatalist->add_data_point(DataPoint::get_instance(get_human_time(cur_bucket), cur_bucket,
                                                            new DataListDataValue(newDatalist), point->get_time()));
            bucketToDataList[cur_bucket] = newDatalist;
        }

        bucketToDataList[cur_bucket]->add_data_point(DataPoint::get_instance(*point));
    }

    delete datalist;
    return splitDatalist;
}

DataList *SplitDatalistDecorator::decorate(DataList *datalist)
{
    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;

        DataList *innerDataList = dynamic_cast<DataListDataValue*>(point->get_value())->get_datalist();

        if (!innerDataList) {
            throw new Error("SplitDatalistDecorator MUST take in a datalist that contains DataListDataValue");
        }
        point->set_value(DataValue::get_instance(decorator->decorate(innerDataList)));
    //    delete innerDataList;
    }

    return datalist;
}


DataList *MergeIDToOne::decorate(DataList *datalist)
{
    for (datalist_iter_t iter = datalist->begin() ;
         iter != datalist->end() ; ++iter) {
        DataPoint *point = *iter;

        point->set_id(1);
    }

    return datalist;
}
