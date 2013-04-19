#include "collection.h"

#include "media_manager.h"

#include "song.h"

Collection::Collection(const QString &name)
    : Music_Item(name), tree_model(0)
{
}

Collection::Collection(const QString &name_, int id_,
           const QList<Music_Item*> &items)
    : Music_Item(name_, id_)
{
    Music_Item *item;
    foreach (item, items)
        add(item);
}

void Collection::add(Music_Item *item)
{
    beginInsertRows(QModelIndex(), count(), count());

    Song *song = dynamic_cast<Song*>(item);

    if (song)
    {
        connect(song, SIGNAL(data_changed()),
                this, SLOT(data_updated()));
    }

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

    beginRemoveRows(QModelIndex(), index, index+1);
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
    start_playing();
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

