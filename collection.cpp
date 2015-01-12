#include "collection.h"

#include "media_manager.h"
#include "library.h"
#include "error.h"
#include "song.h"

#include "database.h"

Collection::Collection(const QString &name, Database *db)
    : Music_Item(name), tree_model(0), database(db)
{
}

Collection::Collection(const QString &name_, int id_, Database *db)
    : Music_Item(name_, id_), database(db)
{
}

Collection::Collection(const QString &name_, int id_,
           const QList<Music_Item*> &items, Database *db)
    : Music_Item(name_, id_), database(db)
{
    insert_items_no_db(items);
}

void Collection::insert_items_no_db(const QList<Music_Item*> &items)
{
    Music_Item *item;
    foreach (item, items)
    {
        if (!item)
            throw Error ("Collection.cpp: Cannot find an item while constructing " + get_name() + "!");

        if (Song *song = dynamic_cast<Song*>(item))
        {
            connect(song, SIGNAL(data_changed()),
                    this, SLOT(data_updated()));
        }

        if (Collection *col = dynamic_cast<Collection*>(item))
            connect(col, SIGNAL(data_changed()),
                    this, SLOT(data_updated()));

        beginInsertRows(QModelIndex(), count(), count());
        children.push_back(item);
        endInsertRows();
    }
}

bool Collection::contains(Music_Item* item)
{
    return children.contains(item);
           // id_to_item.find(item->get_id()) != id_to_item.end();
}

void Collection::add(Music_Item *item)
{
    if (item->get_id() == get_id())
    {
        Error::print_error_msg_str("Cannot insert Collection into itself!");
        return;
    }

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
    }

    // without the check for id==0, this adds playlists to the collection
    // with id 0 on startup causing it to crash.
    if (get_name() != "All Songs" && get_id() != 0 && get_name() != "Library" && database != 0)
        database->add_to_playlist(item->get_id(), get_id());

    if (Collection *col = dynamic_cast<Collection*>(item))
    {
        connect(col, SIGNAL(data_changed()),
                this, SLOT(data_updated()));

    }

    beginInsertRows(QModelIndex(), count(), count());
    children.push_back(item);
    endInsertRows();
}

void Collection::add_items(const std::vector<int> &ids)
{
    foreach (int id, ids)
    {
        add(Media_Manager::get()->get_music_item(id));
    }
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
    qDebug() << "Removing" << index << "from" << get_name();

    if (index == -1 || index > children.size()-1)
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

        if (get_name() != "All Songs" && database != 0)
        {
            qDebug() << "Removing" << song->get_name() << "from DB" << get_name();
            database->delete_from_playlist(song->get_id(), get_id());
        }
    }

    if (Collection *col = dynamic_cast<Collection*>(children[index]))
    {
        disconnect(col, SIGNAL(data_changed()),
                this, SLOT(data_updated()));

        if (get_name() == "Library" && database != 0) {

            qDebug() << "Removing" << col->get_name() << "from DB" << get_name();
            database->delete_playlist(col->get_id());
        }
    }


    beginRemoveRows(QModelIndex(), index, index);
    children.removeAt(index);
    endRemoveRows();
}

const QList<Music_Item *> &Collection::get_children() const
{
    return children;
}

int Collection::get_milliseconds() const
{
    Music_Item *item;
    int sum = 0;

    foreach (item, children)
        sum += item->get_milliseconds();

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

#include "datalist.h"
#include "datapoint.h"

DataList *Collection::convert_to_secCount_datalist()
{
    DataList *list = DataList::get_instance();

    foreach (Music_Item *item , children)
    {
        list->add_data_point(
                    DataPoint::get_instance(item->get_name().toStdString(),
                                            item->get_id(), DataValue::get_instance(item->get_milliseconds()/1000), 0));
    }
    return list;
}

DataList *Collection::convert_to_rating_datalist()
{
    DataList *list = DataList::get_instance();

    foreach (Music_Item *item , children)
    {
        list->add_data_point(
                    DataPoint::get_instance(item->get_name().toStdString(),
                                            item->get_id(), DataValue::get_instance(item->get_rating()), 0));
    }
    return list;
}

void Collection::begin_playing()
{
    Media_Manager::get()->switch_playlist(this);
}

void Collection::select()
{
    Media_Manager::get()->set_center(this);
    emit headerDataChanged(Qt::Horizontal, 0,3);
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
        return 4;

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
                return tr("Rating");
            case 3:
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

void Collection::emit_data_changed()
{
    emit data_changed();
    data_updated();
}

void Collection::move(int i, int pos_to_move_to)
{
    Music_Item *item_to_move = children.at(i);

    children.removeAt(i);
    children.insert(pos_to_move_to, item_to_move);

    emit_data_changed();
}

void Collection::move_up(Music_Item*item)
{
    int i = children.indexOf(item);

    move(i, (i != 0) ? i-1 : children.size()-1);
}
/*
int Collection::get_rating() const
{
    Music_Item *item;
    int sum = 0;
    int count = const_cast<Collection*>(this)->get_leaves().size();

    foreach (item, children)
        sum += item->get_rating();

    return sum/count;
}*/
