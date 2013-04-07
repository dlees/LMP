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
    const QList<Music_Item *> &get_children() const;

    virtual int total_secs() const;
    virtual QLinkedList<Song*> get_leaves();

    QStandardItemModel *get_tree();
    QStandardItemModel *get_list();
    QStandardItemModel *get_table();

    // ListModel functions
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;


private:
    QList<Music_Item*> children;

    // could sort by name? or own sorting Function*?

    QStandardItemModel *tree_model;
    QStandardItemModel *list_model;
    QStandardItemModel *table_model;
};

#endif // COLLECTION_H
