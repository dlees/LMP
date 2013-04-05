#include "button_container.h"
#include "media_manager.h"

Button_Container::Button_Container(QWidget *parent)
{
    play = new QToolButton(this);
    play->setIcon(QIcon(QString::fromUtf8(":/buttons/play.png")));
    play->setGeometry(0, 0, 10, 20);
    connect(play, SIGNAL(clicked()),
            Media_Manager::get(), SLOT(play_cur()));

    this->addWidget(play);
}

Button_Container::~Button_Container()
{

}
