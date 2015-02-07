#ifndef COLLECTION_H
#define COLLECTION_H

#include "music_item.h"

#include <QList>
#include <QtGui>


class Database;
class DataList;

class Collection : public QAbstractTableModel, public Music_Item
{
    Q_OBJECT
public:
    Collection(const QString &name, Database *db);
    Collection(const QString &name_, int id_, Database *db);
    Collection(const QString &name_, int id_,
               const QList<Music_Item*> &items, Database *db);

public:
    bool contains(Music_Item*);

    virtual void add(Music_Item *);
    void add_items(const std::vector<int> &ids);
    void add_leaves(Music_Item *);

    virtual void remove(Music_Item *);
    virtual void remove(int index);

    const QList<Music_Item *> &get_children() const;
    int count()
        {return children.size();}

    virtual int get_milliseconds() const;
    virtual int get_average_ms() const;
    virtual QLinkedList<Song*> get_leaves() ;

    DataList *convert_to_secCount_datalist();
    DataList *convert_to_rating_datalist();

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

    virtual void start_playing();
    virtual void stop_playing();

    virtual void emit_data_changed();

  //  virtual int get_rating() const;

    // moves the ith item
    virtual void move(int i, int pos_to_move_to);
    virtual void move_up(Music_Item *item);

protected:
    void insert_items_no_db(const QList<Music_Item*> &items);

public slots:
    void data_updated();

signals:
    // name changed, playing or not,
    // Doesn't count for changing children
    void data_changed();

private:
    QList<Music_Item*> children;

    QMap<int, Music_Item*> id_to_item;

    QStandardItemModel *tree_model;
    QStandardItemModel *list_model;
    QStandardItemModel *table_model;

    Database *database;
};

#endif // COLLECTION_H
