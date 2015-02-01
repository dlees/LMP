#ifndef DYNAMICPLAYLIST_H
#define DYNAMICPLAYLIST_H

#include "playlist.h"
#include <map>

class Datalist;

class DynamicPlaylist : public Playlist
{
public:
    DynamicPlaylist(time_t start_time, time_t end_time);

    QVariant data(const QModelIndex &index, int role) const;
    QStringList get_headers() const;
private:
    std::map<int, int> id_to_values;
    QList<QVariant> get_overriden_column_data(Music_Item *item) const;
};

#endif // DYNAMICPLAYLIST_H
