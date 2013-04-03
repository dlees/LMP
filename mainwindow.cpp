#include "mainwindow.h"

#include <QtGui>

#include "media_manager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    QWidget *widget = new QWidget;

    QPushButton *butt = new QPushButton("play", widget);

    connect(butt, SIGNAL(clicked())
            , this, SLOT(play()));

    QPushButton *butt2 = new QPushButton("Call Me Maybe", widget);

    connect(butt2, SIGNAL(clicked())
            , this, SLOT(open_and_play()));

    layout->addWidget(butt);
    layout->addWidget(butt2);

    widget->setLayout(layout);

    this->setCentralWidget(widget);

}

MainWindow::~MainWindow()
{
    
}

void MainWindow::play()
{
    manager.play_cur();
}

void MainWindow::open_and_play()
{
    manager.play_new("C:/Call Me Maybe");
}
