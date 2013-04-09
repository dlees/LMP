#ifndef MUSIC_ITEM_H
#define MUSIC_ITEM_H

#include <QString>
#include <QLinkedList>

#include <QtGui>

class Song;

class Music_Item : public QStandardItem
{
public:
    Music_Item(const QString &name);
    virtual ~Music_Item() = 0;

    const QString &get_name() const
        { return name; }

    int get_id() const
        { return id; }

    virtual int total_secs() const = 0;
    virtual QLinkedList<Song*> get_leaves() = 0;

    // have Media Manager start playing this item
    virtual void begin_playing() = 0;

// For being shown in a View
    // adds itself and it's children to the QStandardItem passed in
    virtual void add_child(QStandardItem *) = 0;

    // gets the appropriate headers for this music item
    virtual QStringList get_headers() const = 0;

    virtual QList<QStandardItem*> get_column_data() const = 0;

// Necessary overrides from QStanardItem.
    int type()
       {return UserType;}

    QVariant data(int role) const;

private:
    int id;
    QString name;


    static int max_id;
};

#endif // MUSIC_ITEM_H
