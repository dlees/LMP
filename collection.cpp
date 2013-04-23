#include "collection.h"

#include "media_manager.h"
#include "error.h"
#include "song.h"

#include "database.h"

Collection::Collection(const QString &name)
    : Music_Item(name), tree_model(0)
{
    if (!(name == "Library" || name == "All Songs"))
        Database::get()->new_playlist(get_name(), get_id());
}

Collection::Collection(const QString &name_, int id_,
           const QList<Music_Item*> &items)
    : Music_Item(name_, id_)
{
    Music_Item *item;
    foreach (item, items)
        add(item);
}

bool Collection::contains(Music_Item* item)
{
    return children.contains(item);
           // id_to_item.find(item->get_id()) != id_to_item.end();
}

void Collection::add(Music_Item *item)
{
    if (contains(item))
    {
        Error::print_error_msg_str(get_name() + " already contains "
                                   + item->get_name() + ".\nCan't insert!");
        return;
    }

    if (Song *song = dynamic_cast<Song*>(item))
    {
        connect(song, SIGNAL(data_changed()),
                this, SLOT(data_updated()));    

        if (get_name() != "All Songs")
            Database::get()->add_to_playlist(song->get_id(), this->get_id());
    }

    if (Collection *col = dynamic_cast<Collection*>(item))
        connect(col, SIGNAL(data_changed()),
                this, SLOT(data_updated()));


    beginInsertRows(QModelIndex(), count(), count());
    children.push_back(item);
    endInsertRows();


}

void Collection::data_updated()
{
    emit dataChanged(createIndex(0, 0, 0),
                     createIndex(count(), children.at(0)->get_column_data().size(), 0));
}

void Collection::add_leaves(Music_Item *item)
{
    QLinkedList<Song*> songs = item->get_leaves();
    Song* song;

    foreach (song, songs)
    {
        add(song);
    }
}

void Collection::remove(Music_Item *item)
{
    remove(children.indexOf(item));
}

void Collection::remove(int index)
{
    if (index == -1)
        return;

    if (get_name() == "All Songs")
    {
        Error::print_error_msg_str("Can't remove songs from All Songs");
        return;
    }

    if (children[index]->get_is_playing())
    {
        Error::print_error_msg_str("Can't remove " + children[index]->get_name()
                                   + " while it is being played.");
        return;
    }

    if (Song *song = dynamic_cast<Song*>(children[index]))
    {
        disconnect(song, SIGNAL(data_changed()),
                this, SLOT(data_updated()));

        if (get_name() != "All Songs")
        {
            qDebug() << "Removing" << song->get_name() << "from" << get_name();
            Database::get()->delete_from_playlist(song->get_id(), this->get_id());
        }
    }

    if (Collection *col = dynamic_cast<Collection*>(children[index]))
    {
        disconnect(col, SIGNAL(data_changed()),
                this, SLOT(data_updated()));

        if (get_name() == "Library")
            Database::get()->delete_playlist(col->get_id());
    }


    beginRemoveRows(QModelIndex(), index, index);
    children.removeAt(index);
    endRemoveRows();
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

void Collection::begin_playing()
{
    Media_Manager::get()->switch_playlist(this);
}

void Collection::select()
{
    Media_Manager::get()->set_center(this);
}

void Collection::select_child(int i)
{
    children.at(i)->select();
}

int Collection::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return children.count();
}

int Collection::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (!children.count())
        return 3;

    return children[0]->get_column_data().size();
}

QVariant Collection::data(const QModelIndex &index, int role) const
{
    if (index.row() >= rowCount() || index.row() < 0)
        { return QVariant(); }

    Music_Item *element = children.at(index.row());

    QList<QVariant> columns = element->get_column_data();
    switch (role)
    {
        case Qt::DisplayRole:
            return columns.at(index.column());

        default:
            return QVariant();
    }
}

QVariant Collection::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Name");

            case 1:
                return tr("Seconds");                

            case 2:
                return tr("Artist");

            default:
                return QVariant();
        }
    }
    return QVariant();
}

QStringList Collection::get_headers() const
{
    return Music_Item::get_headers()
            << "Nothing"
               ;
}

QList<QVariant> Collection::get_column_data() const
{
    return Music_Item::get_column_data()
            << ("No Data")
               ;
}

void Collection::start_playing()
{
    qDebug() << get_name() << "starts playing";
    Music_Item::start_playing();
    emit data_changed();
}

void Collection::stop_playing()
{
    Music_Item::stop_playing();
    emit data_changed();
}
