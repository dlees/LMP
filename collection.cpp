#include "collection.h"

Collection::Collection(const QString &name)
    : Music_Item(name)
{
}

void Collection::add(Music_Item *item)
{
    children.insert(item);
}

void Collection::remove(Music_Item *item)
{
    children.remove(item);
}

const QSet<Music_Item *> &Collection::get_children()
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
