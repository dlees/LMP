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
    //widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    Play_Controller *play_controller = new Play_Controller();

    // 1st row containing "Playlist name", searchbar, minimode button
    QLabel *title = new QLabel("Playlist name");
    QLineEdit *search = new QLineEdit("search");
    MiniMode = new QPushButton("m");
    MiniMode->setMaximumWidth(20);

    // Fixed size
    title->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    search->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    MiniMode->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    //2nd row containing Splitter of 3 panes
    // Library Pane
    lib_list = new QListView();
    Pane *libraryPane = new Pane("Library", lib_list);
    lib_list->setModel(Media_Manager::get()->get_library());
    connect(lib_list, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select_lib_item(QModelIndex)));

    /*for (int i = 1; i < Media_Manager::get()->get_library()->columnCount(); i++)
    {
        // Show only the first column for library
        lib_list->hideColumn(i);
    }*/

    // Center Table
    table = new QTableView();
    table->setModel(Media_Manager::get()->get_playlist());
    Pane *centerPane = new Pane("table", table);
    connect(table, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select_center_item(QModelIndex)));

    // Playlist Pane
    playlist = new QListView();
    playlist->setModel(Media_Manager::get()->get_playlist());
    Pane *playlistPane = new Pane("Currently Playing", playlist);
    connect(playlist, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select_song(QModelIndex)));

    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(libraryPane);
    splitter->addWidget(centerPane);
    splitter->addWidget(playlistPane);

    QGridLayout *center_layout = new QGridLayout;
    center_layout->addWidget(title, 0, 1, 1, 3, Qt::AlignLeft);
    center_layout->addWidget(search, 0, 3, 1, 3, Qt::AlignCenter);
    center_layout->addWidget(MiniMode, 0, 6, 1, 1, Qt::AlignCenter);
    center_layout->addWidget(splitter, 1, 0, 1, -1);

    // Add all to main_layout
    QVBoxLayout *main_layout = new QVBoxLayout(widget);
    main_layout->addLayout(center_layout);
    main_layout->addWidget(play_controller);
    widget->setLayout(main_layout);
    this->setCentralWidget(widget);

    connect(MiniMode, SIGNAL(clicked()),
            this, SLOT(send_mini_mode()));
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


// select a song from the playlist pane
// should start playing
void Main_View::select_song(QModelIndex index)
{
    // selecting the child in the playlist that corresponds
    // to the row you clicked will make the song play
    Media_Manager::get()->get_playlist()->select_child(index.row());

}

void Main_View::select_lib_item(QModelIndex index)
{
    // send whatever we selected into the center
    Media_Manager::get()->get_library()->select_child(index.row());

    table->setModel(Media_Manager::get()->get_center());
}

void Main_View::select_center_item(QModelIndex index)
{
    // send whatever we selected into the center
    Media_Manager::get()->get_center()->select_child(index.row());

    table->setModel(Media_Manager::get()->get_center());

}

