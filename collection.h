#ifndef COLLECTION_H
#define COLLECTION_H

#include "music_item.h"

#include <QSet>

class Collection : public Music_Item
{
public:
    Collection(const QString &);

public:
    void add(Music_Item *);
    void remove(Music_Item *);
    const QSet<Music_Item*> &get_children();

private:
    QSet<Music_Item*> children;

    // could sort by name? or own sorting Function*?

};

#endif // COLLECTION_H
