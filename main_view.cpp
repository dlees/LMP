#include "main_view.h"

#include <QtGui>

#include "media_manager.h"
#include "button_container.h"
#include "play_controller.h"
#include "pane.h"
#include "collection.h"

Main_View::Main_View(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;

    Play_Controller *play_controller = new Play_Controller();

    // displays 2 rows of title, and panes
    QGridLayout *center_layout = new QGridLayout;
    QLabel *title = new QLabel("Playlist name");
    center_layout->addWidget(title, 0, 1, 1, 3, Qt::AlignLeft);
    QLineEdit *search = new QLineEdit("search");
    search->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    center_layout->addWidget(search, 0, 3, 1, 3, Qt::AlignCenter);
    MiniMode = new QPushButton("m");
    MiniMode->setMaximumWidth(20);
    center_layout->addWidget(MiniMode, 0, 6, 1, 1, Qt::AlignCenter);

    splitter = new QSplitter(Qt::Horizontal);

    // Library Pane
    QTreeView *tree = new QTreeView();
    Pane *libraryPane = new Pane("Library", tree);
    tree->setModel(Media_Manager::get()->get_library());
    for (int i = 1; i < Media_Manager::get()->get_library()->columnCount(); i++)
    {
        // Show only the first column for libarary
        tree->hideColumn(i);
    }
    splitter->addWidget(libraryPane);

    // Center Table
    QTreeView *test = new QTreeView();
    test->setModel(Media_Manager::get()->get_library());
    Pane *centerPane = new Pane("table", test);
    splitter->addWidget(centerPane);

    // Playlist Pane
    QListView *list = new QListView();
    list->setModel(Media_Manager::get()->get_playlist());
    Pane *playlistPane = new Pane("Currently Playing", list);
    splitter->addWidget(playlistPane);

    center_layout->addWidget(splitter, 1, 0, 1, -1);



    // Add all to main_layout
    QVBoxLayout *main_layout = new QVBoxLayout(widget);
    main_layout->addLayout(center_layout);
    main_layout->addWidget(play_controller);
    widget->setLayout(main_layout);
    this->setCentralWidget(widget);

    //create_menu();

    Media_Manager::get()->start_up();

    connect(MiniMode, SIGNAL(clicked()),
            this, SLOT(send_mini_mode()));

}

void Main_View::create_menu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    connect(add_menu_item((char*)"Add song to current playlist", true), SIGNAL(triggered()),
            Media_Manager::get(), SLOT(add_cur_to_playlist()));
    connect(add_menu_item((char*)"&Open song", true), SIGNAL(triggered()),
            this, SLOT(add_files()));
    connect(add_menu_item((char*)"&Exit", true), SIGNAL(triggered()),
            this, SLOT(quit()));

    editMenu = menuBar()->addMenu(tr("&Edit"));
}

QAction *Main_View::add_menu_item(char name[], bool enabled)
{
    QAction *newAct = new QAction(tr(name), this);
    newAct->setEnabled(enabled);
    fileMenu->addAction(newAct);

    return newAct;
}

Main_View::~Main_View()
{

}

void Main_View::play()
{
    Media_Manager::get()->play_cur();
}

void Main_View::open_and_play()
{
    Media_Manager::get()->play_new("C:/Call Me Maybe");
}

void Main_View::add_files()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Music Files"),
        QDesktopServices::storageLocation(QDesktopServices::MusicLocation));

    if (files.isEmpty())
        return;

    foreach (QString filename, files)
    {
        qDebug() << filename << endl;
    }
    Media_Manager::get()->play_new(files.at(0));
}

void Main_View::quit()
{
    QApplication::quit();
}

void Main_View::send_mini_mode()
{
    qDebug() << "SEND_MINI_MODE";

    emit to_mini_mode();
}
