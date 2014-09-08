#ifndef ANALYSER_H
#define ANALYSER_H

#include <string>
#include <vector>
#include <map>

#include <QtGui>

class Datatype
{
public:
    int get_time() const {return time;} // x axis,
    int get_value() const {return value;} // y axis

    bool operator<(const Datatype &B) const
    {
        return this->get_time() < B.get_time();
    }

private:
    int time;
    int value;
};

class SecCount : public Datatype
{
public:
    int get_time() {return timestamp_on_save;}
    int get_value() {return end_position-start_position;}

private:
    int timestamp_on_save;
    int start_position;
    int end_position;

};

// Song would be Data where label=name; datatypes are secCounts or ratingCounts
// Database entry would be where label=songname <- Can't use for datatype
class Data
{
public:
    static Data* create_instance() {return new Data;}

    std::string label;
    std::vector<Datatype> datatypes;

    // inefficient
    void filter(int time_start, int time_end)
    {
        std::vector<Datatype> filtered_datatypes;
        foreach (Datatype dt, datatypes)
        {
            if (dt.get_time() < time_end && dt.get_time() > time_start)
                filtered_datatypes.push_back(dt);
        }
        datatypes = filtered_datatypes;
    }

    // Encapsulated Constructor
private:
    Data();
};

class DataDecorator
{
public:
    std::vector<Data*> *decorate(std::vector<Data*> *data_list);

};

// A type of Decorator for data
// It goes through the list and transforms each data
class Transform
{
public:
    std::vector<Data*> *decorate(std::vector<Data*> *data_list)
    {
        foreach (Data *data, *data_list) {
            transform_data(data);
        }
        return data_list;
    }

protected:
    virtual Data* transform_data(Data *data); //template method
};

class TimeFilterTransform : public Transform
{
public:
    TimeFilterTransform(int start_time_, int end_time_) :
        start_time(start_time_), end_time(end_time_)
    {}

protected:
    virtual Data *transform_data(Data *data) {
        data->filter(start_time, end_time);
        return data;
    }

private:
    int start_time;
    int end_time;
};

#endif // ANALYSER_H
