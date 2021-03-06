#include "catalog.h"
#include "database.h"

Catalog::Catalog(const QString &name, Database *db)
    : Collection(name, db)
{
    if (db)
        db->new_playlist(get_name(), get_id(), true);
}

Catalog::Catalog(const QString &name, int id_)
    : Collection(name, id_, Database::get())
{}

void Catalog::insert_items_no_db(const QList<Music_Item *> &items)
{
    Collection::insert_items_no_db(items);
}
