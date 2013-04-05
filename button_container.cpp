#include "button_container.h"

Button_Container::Button_Container(QWidget *parent)
{
    QToolBar *toolBar = new QToolBar(parent);
    QHBoxLayout *layout = new QHBoxLayout(toolBar);
    //QHBoxLayout *layout = new QHBoxLayout(this);

    play = new QToolButton(toolBar);
    //pause = new QToolButton(toolBar);
    //play = new QToolButton(this);
    play->setIcon(QIcon(QString::fromUtf8(":/buttons/play.png")));

    layout->addWidget(play);
    //layout->addWidget(pause);
    this->setLayout(layout);
    //this->setCentralWidget(toolBar);
}

Button_Container::~Button_Container()
{

}
