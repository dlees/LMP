#ifndef MUSIC_ITEM_H
#define MUSIC_ITEM_H

#include <QString>

class Music_Item
{
public:
    Music_Item(const QString &name);
    virtual ~Music_Item() = 0;

    const QString &get_name()
        { return name; }

    int get_id()
        { return id; }

private:
    int id;
    QString name;


    static int max_id;
};

#endif // MUSIC_ITEM_H
