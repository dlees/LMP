#include "collection.h"

#include "media_manager.h"

Collection::Collection(const QString &name)
    : Music_Item(name), tree_model(0)
{
}

void Collection::add(Music_Item *item)
{
    children.push_back(item);

    if (tree_model)
        tree_model->appendRow(item);
}

void Collection::remove(Music_Item *item)
{
    children.removeOne(item);
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

int Collection::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return children.count();
}

QVariant Collection::data(const QModelIndex &index, int role) const
{
    if (index.row() >= rowCount() || index.row() < 0)
        { return QVariant(); }

    Music_Item *element = children.at(index.row());

    switch (role)
    {
        case Qt::DisplayRole:
            return element->get_name();

        default:
            return QVariant();
    }
}

QStandardItemModel *Collection::get_model()
{
    Music_Item *item;

    tree_model = new QStandardItemModel();

    if (children.size())
        tree_model->setHorizontalHeaderLabels(children.at(0)->get_headers());

    foreach (item, children)
    {
        QList<QStandardItem*> columns(item->get_column_data());

        tree_model->appendRow(columns);

        item->add_child(columns.at(0));
    }

    return tree_model;
}

void Collection::add_child(QStandardItem *parent)
{
    Music_Item *item;

    foreach (item, children)
    {
        QList<QStandardItem*> columns(item->get_column_data());

        parent->appendRow(columns);

        item->add_child(columns.at(0));
    }
}

QStringList Collection::get_headers() const
{
    return Music_Item::get_headers()
            << "Nothing"
               ;
}

QList<QStandardItem*> Collection::get_column_data() const
{
    return Music_Item::get_column_data()
            << new QStandardItem("No Data")
               ;
}
