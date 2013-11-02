#ifndef MUSIC_ITEM_H
#define MUSIC_ITEM_H

#include <QString>
#include <QLinkedList>

#include <QtGui>

#include "star_rating.h"

class Song;

class Music_Item
{
public:
    Music_Item(const QString &name);
    Music_Item(const QString &name_, int id_, int rating = 2);

    virtual ~Music_Item() = 0;

    const QString &get_name() const
        { return name; }

    int get_id() const
        { return id; }

    virtual int total_millisecs() const = 0;
    virtual QLinkedList<Song*> get_leaves() = 0;

    // have Media Manager start playing this item
    virtual void begin_playing() = 0;

    virtual void start_playing()
        { is_playing = true;}
    virtual void stop_playing()
        { is_playing = false;}
    bool get_is_playing()
        {return is_playing;}

    virtual void change_rating(int new_rating);

    // select this item
    virtual void select()
        {begin_playing();}

// For being shown in a View
    // gets the appropriate headers for this music item
    virtual QStringList get_headers() const = 0;

    virtual QList<QVariant> get_column_data() const = 0;

    virtual void emit_data_changed() = 0;

    static int max_id;

    // template method pattern, composite pattern
    Star_Rating get_star_rating() const;
    virtual int get_rating() const;



protected:
    QString name;

private:
    bool is_playing;

    int id;

    int cur_rating;


};

#endif // MUSIC_ITEM_H
