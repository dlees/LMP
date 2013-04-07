#include "collection.h"

Collection::Collection(const QString &name)
    : Music_Item(name)
{
}

void Collection::add(Music_Item *item)
{
    children.push_back(item);
}

void Collection::remove(Music_Item *item)
{
    children.removeOne(item);
}

const QList<Music_Item *> &Collection::get_children() const
{
    return children;
}

int Collection::total_secs() const
{
    Music_Item *item;
    int sum = 0;

    foreach (item, children)
        sum += item->total_secs();

    return sum;
}


QLinkedList<Song*> Collection::get_leaves()
{
    Music_Item *item;
    QLinkedList<Song*> leaves;

    foreach (item, children)
        leaves += item->get_leaves();

    return leaves;
}

int Collection::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return children.count();
}

QVariant Collection::data(const QModelIndex &index, int role) const
{
    if (index.row() >= rowCount() || index.row() < 0)
        { return QVariant(); }

    Music_Item *element = children.at(index.row());

    switch (role)
    {
        case Qt::DisplayRole:
            return element->get_name();

        default:
            return QVariant();
    }
}

QStandardItemModel *Collection::get_tree()
{
    Music_Item *item;

    tree_model = new QStandardItemModel(children.size(),1);

    foreach (item, children)
        tree_model->appendRow(item);

    return tree_model;
}

QStandardItemModel *Collection::get_list()
{
    Music_Item *item;

    list_model = new QStandardItemModel(children.size(),1);

    foreach (item, children)
        list_model->appendRow(item);

    return list_model;
}

QStandardItemModel *Collection::get_table()
{
    return 0;
}

