#ifndef COLLECTION_H
#define COLLECTION_H

#include "music_item.h"

#include <QList>
#include <QtGui>

class Collection : public Music_Item, public QAbstractListModel
{
public:
    Collection(const QString &);

public:
    virtual void add(Music_Item *);
    void remove(Music_Item *);
    const QList<Music_Item *> &get_children();

    virtual int total_secs();
    virtual QLinkedList<Song*> get_leaves();


    // ListModel functions
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

private:
    QList<Music_Item*> children;

    // could sort by name? or own sorting Function*?

};

#endif // COLLECTION_H
