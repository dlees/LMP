#include "mainwindow.h"

#include <QtGui>

#include "media_manager.h"
#include "button_container.h"
#include "play_controller.h"
#include "pane.h"

#include "collection.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;

    // displays title of center view, searchbar
    QHBoxLayout *header_layout = new QHBoxLayout;
    QLabel *title = new QLabel("title");
    header_layout->addWidget(title);
    QLabel *search = new QLabel("searchbar");
    header_layout->addWidget(search);


    QHBoxLayout *centerLayout = new QHBoxLayout;
    QTreeView *tree = new QTreeView();
    Pane *libraryPane = new Pane(tree);
    tree->setModel(Media_Manager::get()->get_library());
    centerLayout->addWidget(libraryPane);
    Pane *centerPane = new Pane(NULL);
    centerLayout->addWidget(centerPane);


    QListView *list = new QListView();
    list->setModel(Media_Manager::get()->get_playlist());
    Pane *playlistPane = new Pane(list);
    centerLayout->addWidget(playlistPane);

    Play_Controller *play_controller = new Play_Controller();

    QPushButton *butt2 = new QPushButton("Call Me Maybe", widget);

    connect(butt2, SIGNAL(clicked())
            , this, SLOT(open_and_play()));

    // Add to main_layout
    QVBoxLayout *main_layout = new QVBoxLayout(widget);
    main_layout->addLayout(header_layout);
    main_layout->addLayout(centerLayout);
    main_layout->addWidget(play_controller);
    main_layout->addWidget(butt2);

    widget->setLayout(main_layout);
    this->setCentralWidget(widget);

    create_menu();
}

void MainWindow::create_menu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    connect(add_menu_item((char*)"Add song to current playlist", true), SIGNAL(triggered()),
            Media_Manager::get(), SLOT(add_cur_to_playlist()));
    connect(add_menu_item((char*)"&Exit", true), SIGNAL(triggered()),
            this, SLOT(quit()));

    editMenu = menuBar()->addMenu(tr("&Edit"));
}

QAction *MainWindow::add_menu_item(char name[], bool enabled)
{
    QAction *newAct = new QAction(tr(name), this);
    newAct->setEnabled(enabled);
    fileMenu->addAction(newAct);

    return newAct;
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

void MainWindow::quit()
{
    QApplication::quit();
}
