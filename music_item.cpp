#include "music_item.h"

int Music_Item::max_id = -1;

Music_Item::Music_Item(const QString &name_)
    : name(name_),
    id(++max_id)
{

}

Music_Item::~Music_Item()
{

}
