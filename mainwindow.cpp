#include "mainwindow.h"

#include <QtGui>

#include "media_manager.h"
#include "button_container.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);

    QHBoxLayout *centerLayout = new QHBoxLayout;
    QLabel *label_placeholder = new QLabel("Center panes");
    centerLayout->addWidget(label_placeholder);

    Button_Container *button_container = new Button_Container(100, 30);

    QPushButton *butt2 = new QPushButton("Call Me Maybe", widget);

    connect(butt2, SIGNAL(clicked())
            , this, SLOT(open_and_play()));

    mainLayout->addLayout(centerLayout);
    mainLayout->addWidget(button_container);
    //mainLayout.addWidget(play_controller);
    mainLayout->addWidget(butt2);

    widget->setLayout(mainLayout);

    this->setCentralWidget(widget);
    setupMenu();
}

void MainWindow::setupMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
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
