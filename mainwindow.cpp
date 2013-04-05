#include "mainwindow.h"

#include <QtGui>

#include "media_manager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QWidget *widget = new QWidget;

    QHBoxLayout *layout = new QHBoxLayout(widget);

    QToolButton *butt = new QToolButton(widget);
    QIcon icon;
    QPixmap pixmap(":/buttons/pause.png");
    icon.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
    butt->setIcon(icon);

    //QPushButton *butt = new QPushButton("play", widget);

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
    Media_Manager::get()->play_cur();
}

void MainWindow::open_and_play()
{
    Media_Manager::get()->play_new("C:/Call Me Maybe");
}
