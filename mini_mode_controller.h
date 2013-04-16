#ifndef MINI_MODE_CONTROLLER_H
#define MINI_MODE_CONTROLLER_H

#include <QFrame>
#include <QSlider>
#include "button_container.h"

class Mini_Mode_Controller : public QFrame
{
    Q_OBJECT

public:
    Mini_Mode_Controller();
    ~Mini_Mode_Controller();

private:
    Button_Container *button_container;
    QSlider *slider;

public slots:
    /*
    void set_slider_position(qint64 value);
    void send_new_position();
    void set_total_value(int time);
    */
};

#endif // MINI_MODE_CONTROLLER_H
