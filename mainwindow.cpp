#include "mainwindow.h"

#include <QtGui>

#include "media_manager.h"
#include "button_container.h"
#include "play_controller.h"
#include "pane.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;

    QHBoxLayout *topLayout = new QHBoxLayout;
    QLabel *title = new QLabel("title");
    topLayout->addWidget(title);
    QLabel *search = new QLabel("searchbar");
    topLayout->addWidget(search);

    QHBoxLayout *centerLayout = new QHBoxLayout;
    QTreeView *tree = new QTreeView();
    Pane *libraryPane = new Pane(tree);
    centerLayout->addWidget(libraryPane);
    Pane *centerPane = new Pane(NULL);
    centerLayout->addWidget(centerPane);
    QListView *list = new QListView();
    Pane *playlistPane = new Pane(list);
    centerLayout->addWidget(playlistPane);

    Play_Controller *play_controller = new Play_Controller();

    QPushButton *butt2 = new QPushButton("Call Me Maybe", widget);

    connect(butt2, SIGNAL(clicked())
            , this, SLOT(open_and_play()));

    QVBoxLayout *mainLayout = new QVBoxLayout(widget);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(centerLayout);
    mainLayout->addWidget(play_controller);
    mainLayout->addWidget(butt2);

    widget->setLayout(mainLayout);
    this->setCentralWidget(widget);
    create_menu();
}

void MainWindow::create_menu()
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
