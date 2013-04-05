#include "mainwindow.h"

#include <QtGui>

#include "media_manager.h"
#include "button_container.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QWidget *widget = new QWidget;

    QVBoxLayout *layout = new QVBoxLayout(widget);

    Button_Container *button_container = new Button_Container(100, 30);

    QPushButton *butt2 = new QPushButton("Call Me Maybe", widget);

    connect(butt2, SIGNAL(clicked())
            , this, SLOT(open_and_play()));

    layout->addWidget(button_container);

    layout->addWidget(butt2);

    widget->setLayout(layout);

    this->setCentralWidget(widget);

}

MainWindow::~MainWindow()
{
    
}

void MainWindow::play()
{
    Media_Manager::get()->play_cur();
}

void MainWindow::open_and_play()
{
    Media_Manager::get()->play_new("C:/Call Me Maybe");
}
