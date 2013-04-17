#ifndef PLAY_CONTROLLER_H
#define PLAY_CONTROLLER_H

#include <QFrame>
#include <QSlider>
#include <QLabel>
#include "button_container.h"
#include "song.h"

class Play_Controller : public QFrame
{
    Q_OBJECT

public:
    Play_Controller();
    ~Play_Controller();

private:
    Button_Container *button_container;
    QSlider *slider;
    QLabel *song_title;
    QLabel *HotSpots;
    QLabel *time_view;
    Song *cur_song;
    QString convert_ms_to_min(int ms);
    void initialize_slider();

public slots:
    void set_slider_position(qint64 value);
    void send_new_position();
    void set_total_value(int time);
    void set_song(Song *song);
    //void set_new_song(Song *song, int time);
    void set_hs_text();
    void set_song_title();
};

#endif // PLAY_CONTROLLER_H
