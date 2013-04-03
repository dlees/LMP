#include "collection.h"

Collection::Collection()
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
