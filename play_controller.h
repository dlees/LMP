#ifndef PLAY_CONTROLLER_H
#define PLAY_CONTROLLER_H

#include <QFrame>
#include "button_container.h"

class Play_Controller : public QFrame
{
    Q_OBJECT

public:
    Play_Controller();
    ~Play_Controller();

private:
    Button_Container *button_container;

};

#endif // PLAY_CONTROLLER_H
