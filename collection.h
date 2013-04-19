#ifndef COLLECTION_H
#define COLLECTION_H

#include "music_item.h"

#include <QList>
#include <QtGui>

class Collection : public QAbstractTableModel, public Music_Item
{
    Q_OBJECT
public:
    Collection(const QString &name);
    Collection(const QString &name_, int id_,
               const QList<Music_Item*> &items);

public:
    virtual void add(Music_Item *);
    void add_leaves(Music_Item *);

    virtual void remove(Music_Item *);
    virtual void remove(int index);

    const QList<Music_Item *> &get_children() const;
    int count()
        {return children.size();}

    virtual int total_secs() const;
    virtual QLinkedList<Song*> get_leaves();

    QStandardItemModel *get_model();

    virtual void begin_playing();

    // put this collection in the center table
    virtual void select();

    // gets the appropriate headers for this music item
    virtual QStringList get_headers() const;

    virtual QList<QVariant> get_column_data() const;

    virtual void select_child(int i);

// TableModel functions
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void data_updated();

private:
    QList<Music_Item*> children;

    // could sort by name? or own sorting Function*?

    QStandardItemModel *tree_model;
    QStandardItemModel *list_model;
    QStandardItemModel *table_model;
};

#endif // COLLECTION_H
