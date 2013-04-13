#include "hotspots.h"

#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <QDebug>

#include "error.h"
//#include "Utility.h"

using namespace std;

Hotspots::Hotspots(void)
{
}


Hotspots::~Hotspots(void)
{
}

void Hotspots::add(int hotspot)
{
    // find where t should be located
    vector<int>::iterator it;
    it = lower_bound(hotspots.begin(), hotspots.end(), hotspot);

    // if t is already present
    if (it != hotspots.end() && *it == hotspot)
        qDebug() << "ERROR: ALready a Hotspot";
        //throw Error(L"Already a Hotspot");


    // insert t at correct position
    hotspots.insert(it, hotspot);
}

void Hotspots::remove(int hotspot)
{
    hotspots.erase(find(hotspot));
}

vector<int>::iterator Hotspots::find(int hs)
{
    // find where t should be located
    vector<int>::iterator it;
    it = lower_bound(hotspots.begin(), hotspots.end(), hs);

    // if it is not present
    if (it == hotspots.end())
        qDebug() << "ERROR: No Hotspot at that Time";
        //throw Error(L"No Hotspot at that Time");

    return it;
}

int Hotspots::get_next(int spot)
{
    return *(find(spot)); // find uses lower_bound
}

int Hotspots::get_prev(int spot)
{
    try
    {
        vector<int>::iterator it = find(spot);

        // if it is the first hotspot, go to 0
        if (it == hotspots.begin())
            return 0;
        else
            return *(--it);
    }
    catch (Error) // it is the end of the
    {
        if (!hotspots.size())
            return 0;

        return hotspots.at(hotspots.size()-1);
    }
}

void Hotspots::save(std::wofstream &fout)
{
    fout << hotspots.size() << endl;

    for (vector<int>::iterator i = hotspots.begin() ;
        i != hotspots.end() ; ++i)
    {
        fout << *i << endl;
    }
}

void Hotspots::load(std::wifstream &fin)
{
    int num;
    fin >> num;

    while (num--)
    {
        int h;
        fin >> h;
        add(h);
    }
}

Hotspots &Hotspot_map::get_hotspots(int song_id)
{
    if (hotspot_map.find(song_id) == hotspot_map.end())
        qDebug() << "ERROR: Song has no hotspots";
        //throw Error(L"Song Has No Hotspots.");

    return hotspot_map.at(song_id);
}

void Hotspot_map::add_hotspot(int song_id, qint64 hotspot)
{
    hotspot_map[song_id].add(hotspot);
}


Hotspot_map::Hotspot_map()
{
    //load(def_path_c "hotspots.txt");
}

Hotspot_map::~Hotspot_map()
{
    //save(def_path_c "hotspots.txt");
}

void Hotspot_map::save(char *filename)
{
    wofstream fout(filename);

    fout << hotspot_map.size() << endl;

    for (hotspot_map_t::iterator i = hotspot_map.begin() ;
            i != hotspot_map.end() ; ++i)
    {
        fout << i->first << endl;
        i->second.save(fout);
    }
    fout.close();
}

void Hotspot_map::load(char *filename)
{
    wifstream fin(filename);

    if (fin.fail())
    {
        return ;//throw Error(L"Hotspots Failed!");
    }

    hotspot_map.clear();

    int num;
    fin >> num;

    if (fin.fail())
        qDebug() << "ERROR: Hotspots failed! Num";
        //throw Error(L"Hotspots Failed! Num");
/*
    while (num--)
    {
        qint64 song;
        Hotspots hs;

        fin.get(); // get '\n'
        getline(fin, song);

        if (fin.fail())
            qDebug() << "ERROR: Hotspots Failed";
            //throw Error(L"Hotspots Failed!");

        hs.load(fin);

        hotspot_map[song] = hs;
    }
    */
    fin.close();
}
