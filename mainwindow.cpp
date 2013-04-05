#include "mainwindow.h"

#include <QtGui>

#include "media_manager.h"
#include "button_container.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QWidget *widget = new QWidget;

    QVBoxLayout *layout = new QVBoxLayout(widget);

    Button_Container *button_container = new Button_Container(this);
    //QToolButton *butt = new QToolButton(widget);
    /*QIcon icon;
    QPixmap pixmap(":/buttons/play.png");
    icon.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
    butt->setIcon(icon);
    */

    //butt->setIcon(QIcon(QString::fromUtf8(":/buttons/play.png")));

   // connect(butt, SIGNAL(clicked())
     //       , this, SLOT(play()));

    QPushButton *butt2 = new QPushButton("Call Me Maybe", widget);

    connect(butt2, SIGNAL(clicked())
            , this, SLOT(open_and_play()));

    layout->addWidget(button_container);
   // layout->addWidget(butt);
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
