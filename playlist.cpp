#include "playlist.h"

#include "media_manager.h"
#include "song.h"
#include "error.h"


Playlist::Playlist(const QString &name)
    : Collection(name),
      cur(0)
{
}

Playlist::Playlist(Collection *col)
    : Collection(col->get_name()+"_playlist"),
      cur(0)
{
   Music_Item *item;
   foreach (item, col->get_children())
       add(item);
}


Playlist::Playlist(const QString &name, int id_,
         const QList<Music_Item*> &items)
    : Collection(name, id_, items),
      cur(0)
{}

void Playlist::add(Music_Item *item)
{
    QLinkedList<Song*> songs = item->get_leaves();
    Song* song;

    foreach (song, songs)
    {
        Collection::add(song);        
    }
    cur = Collection::count() - 1;
}

void Playlist::select_child(int i)
{
    Collection::select_child(i);
    cur = i;
}

void Playlist::begin_playing()
{
    qDebug() << get_name() << "playing";
    Media_Manager::get()->switch_playlist(this);
}

void Playlist::prev()
{
    if (count() == 0)
    {
        Error::print_error_msg_str("No songs yet! Add songs to this playlist!");
        return;
    }
    get_children().at(cur = (cur+count()-1)%count())->begin_playing();
}

void Playlist::next()
{
    if (count() == 0)
    {
        Error::print_error_msg_str("No songs yet! Add songs to this playlist!");
        return;
    }
    get_children().at(cur = (cur+1)%count())->begin_playing();
}

void Playlist::set_cur(int index)
{
    cur = index;
}

Playlist::~Playlist()
{

}
