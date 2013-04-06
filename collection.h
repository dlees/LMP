#ifndef COLLECTION_H
#define COLLECTION_H

#include "music_item.h"

#include <QSet>

class Collection : public Music_Item
{
public:
    Collection(const QString &);

public:
    virtual void add(Music_Item *);
    void remove(Music_Item *);
    const QSet<Music_Item*> &get_children();

    virtual int total_secs();

    virtual QLinkedList<Song*> get_leaves();

private:
    QSet<Music_Item*> children;

    // could sort by name? or own sorting Function*?

};

#endif // COLLECTION_H
