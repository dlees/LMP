#ifndef HANDLE_DATA_VALUE_H
#define HANDLE_DATA_VALUE_H

#include "datalist.h"
#include "datapoint.h"
#include <QDateTime>

 class Handle_Count {
 public:
     virtual void handle(int ID, DataValue* value, const QString &time) = 0;
 };

 class Add_Count_To_DataList : public Handle_Count {
 public:
     Add_Count_To_DataList(DataList *datalist_)
         : list(datalist_) {}

     void handle(int ID, DataValue* value, const QString &time){
         list->add_data_point(
                     DataPoint::get_instance("name", ID, value, QDateTime::fromString(time).toTime_t()));
     }

 private:
      DataList *list;
 };

 class Map_ID_to_Total_Value : public Handle_Count {
 public:
     Map_ID_to_Total_Value(QMap<int, int> &songIDToTotal_)
         : songIDToTotalValue(songIDToTotal_) {}

     void handle(int ID, DataValue* value, const QString &){
         if (songIDToTotalValue.find(ID) == songIDToTotalValue.end())
             songIDToTotalValue[ID] = 0;

         int total = songIDToTotalValue[ID];

         songIDToTotalValue[ID] = total + value->get_value();
         delete value;
     }

 private:
      QMap<int, int> &songIDToTotalValue;
 };

 class Map_ID_to_Value : public Handle_Count {
 public:
     Map_ID_to_Value(QMap<int, int> &songIDToValue_)
         : songIDToRating(songIDToValue_) {}

     void handle(int ID, DataValue* value, const QString &){
         songIDToRating[ID] = value->get_value();
         delete value;
     }

 private:
      QMap<int, int> &songIDToRating;
 };
#endif // HANDLE_DATA_VALUE_H
