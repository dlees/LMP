#ifndef PLAY_CONTROLLER_H
#define PLAY_CONTROLLER_H

#include <QFrame>
#include <QSlider>
#include "button_container.h"

class Play_Controller : public QFrame
{
    Q_OBJECT

public:
    Play_Controller();
    ~Play_Controller();

private:
    Button_Container *button_container;
    QSlider *slider;

public slots:
    void set_slider_position(qint64 value);
};

#endif // PLAY_CONTROLLER_H
