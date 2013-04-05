#include "mainwindow.h"

#include <QtGui>

#include "media_manager.h"
#include "button_container.h"
#include "play_controller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);

    QHBoxLayout *centerLayout = new QHBoxLayout;
    QLabel *leftPane = new QLabel("Left Pane");
    QLabel *centerPane = new QLabel("Center Pane");
    QLabel *rightPane = new QLabel("Right Pane");
    leftPane->setFrameStyle(QFrame::Panel | QFrame::Raised);
    leftPane->setLineWidth(1);
   centerPane->setFrameStyle(QFrame::Panel | QFrame::Raised);
    centerPane->setLineWidth(1);
    rightPane->setFrameStyle(QFrame::Panel | QFrame::Raised);
    rightPane->setLineWidth(1);
    centerLayout->addWidget(leftPane);
    centerLayout->addWidget(centerPane);
    centerLayout->addWidget(rightPane);

    Play_Controller *play_controller = new Play_Controller();
    //Button_Container *button_container = new Button_Container(100, 30);

    QPushButton *butt2 = new QPushButton("Call Me Maybe", widget);

    connect(butt2, SIGNAL(clicked())
            , this, SLOT(open_and_play()));

    mainLayout->addLayout(centerLayout);
    mainLayout->addWidget(play_controller);

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
