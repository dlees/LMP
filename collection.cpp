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

const QList<Music_Item *> &Collection::get_children()
{
    return children;
}

int Collection::total_secs()
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


