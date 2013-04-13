#ifndef HOTSPOTS_H
#define HOTSPOTS_H

#pragma once

#include <map>
#include <vector>
#include <fstream>

#include <QtGui>

// IF SLOW DOWN OCCURS:
// Consider Hotspot cache.
// the nice thing is these classes don't need to be modified
// besides possibly adding a remove song function

class Hotspots
{
public:
    Hotspots(void);
    ~Hotspots(void);

    void add(int hotspot);
    void remove(int hotspot);

    int get_next(int spot);
    int get_prev(int spot);

    void save(std::wofstream &fout);
    void load(std::wifstream &fin);

private:
    std::vector<int> hotspots;

    std::vector<int>::iterator find(int hs);
};

class Hotspot_map
{
public:
    Hotspot_map();
    ~Hotspot_map();

    Hotspots &get_hotspots(int song_id);
    void add_hotspot(int song_id, qint64 hotspot);

    void save(char *filename);
    void load(char *filename);

private:
    typedef std::map<int, Hotspots> hotspot_map_t;

    hotspot_map_t hotspot_map;

};

#endif // HOTSPOTS_H
