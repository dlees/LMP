#ifndef MUSIC_ITEM_H
#define MUSIC_ITEM_H

#include <QString>
#include <QLinkedList>

class Song;

class Music_Item
{
public:
    Music_Item(const QString &name);
    virtual ~Music_Item() = 0;

    const QString &get_name()
        { return name; }

    int get_id()
        { return id; }

    virtual int total_secs() = 0;
    virtual QLinkedList<Song*> get_leaves() = 0;

private:
    int id;
    QString name;


    static int max_id;
};

#endif // MUSIC_ITEM_H
