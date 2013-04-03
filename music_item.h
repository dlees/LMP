#ifndef MUSIC_ITEM_H
#define MUSIC_ITEM_H

#include <QString>

class Music_Item
{
public:
    Music_Item();    
    virtual ~Music_Item() = 0 {}

    const QString &get_name()
        { return name; }



private:
    QString name;

};

#endif // MUSIC_ITEM_H
