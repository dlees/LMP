#include "catalog.h"
#include "database.h"

Catalog::Catalog(const QString &name)
    : Collection(name)
{
    Database::get()->new_playlist(get_name(), get_id(), true);
}

Catalog::Catalog(const QString &name, int id_,
         const QList<Music_Item*> &items)
    : Collection(name, id_, items)
{}
