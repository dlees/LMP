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
    int count()
        {return children.size();}

    virtual int total_secs() const;
    virtual QLinkedList<Song*> get_leaves();

    QStandardItemModel *get_tree();
    QStandardItemModel *get_list();
    QStandardItemModel *get_table();

     virtual void begin_playing();

    virtual void add_child(QStandardItem *);

    // gets the appropriate headers for this music item
    virtual QStringList get_headers() const;

    virtual QList<QStandardItem*> get_column_data() const;



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
