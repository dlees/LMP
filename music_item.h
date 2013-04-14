#ifndef MUSIC_ITEM_H
#define MUSIC_ITEM_H

#include <QString>
#include <QLinkedList>

#include <QtGui>

class Song;

class Music_Item
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

    // select this item
    virtual void select()
        {begin_playing();}

// For being shown in a View
    // gets the appropriate headers for this music item
    virtual QStringList get_headers() const = 0;

    virtual QList<QVariant> get_column_data() const = 0;

protected:
    QString name;

private:
    int id;


    static int max_id;
};

#endif // MUSIC_ITEM_H
