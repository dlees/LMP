#ifndef CATALOG_H
#define CATALOG_H

#include "collection.h"

/*
    A Catalog is a Collection that only contains collections
*/

class Catalog : public Collection
{
public:
    Catalog(const QString &name);
    Catalog(const QString &name, int id_);

    void insert_items_no_db(const QList<Music_Item*> &items);
};

#endif // CATALOG_H
