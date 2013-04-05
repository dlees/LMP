#include <QVBoxLayout>

#include "play_controller.h"
#include "button_container.h"
#include <QSlider>

Play_Controller::Play_Controller()
    : button_container(new Button_Container(100, 30))
{
    QVBoxLayout *layout = new QVBoxLayout();
    QSlider *slider = new QSlider(Qt::Horizontal);
    layout->addWidget(slider);
    layout->addWidget(button_container);
    setLayout(layout);
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);
}

Play_Controller::~Play_Controller()
{

}
