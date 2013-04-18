#include "main_view.h"

#include <QtGui>

#include "media_manager.h"
#include "button_container.h"
#include "play_controller.h"
#include "pane.h"
#include "collection.h"
#include "playlist.h"

Main_View::Main_View(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;

    Play_Controller *play_controller = new Play_Controller();

    // 1st row containing "Playlist name", searchbar, minimode button
    QLineEdit *search = new QLineEdit("search");
    MiniMode = new QPushButton("-");
    MiniMode->setToolTip("Switch to MiniMode");
    //MiniMode->setMaximumWidth(20);

    //2nd row containing Splitter of 3 panes
    // Library Pane
    lib_list = new QListView();
    Pane *libraryPane = new Pane("Library", lib_list);
    lib_list->setModel(Media_Manager::get()->get_library());
    connect(lib_list, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select_lib_item(QModelIndex)));

    // Center Table
    table = new QTableView();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSortingEnabled(true);
    table->setShowGrid(false);
    table->setModel(Media_Manager::get()->get_playlist());
    table->setColumnWidth(0, 200);
    Pane *centerPane = new Pane("New Playlist", table);
    connect(table, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select_center_item(QModelIndex)));

    // Playlist Pane
    playlist = new QListView();
    playlist->setModel(Media_Manager::get()->get_playlist());
    Pane *playlistPane = new Pane("Current Playlist", playlist);
    connect(playlist, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select_song(QModelIndex)));

    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(libraryPane);
    splitter->addWidget(centerPane);
    splitter->addWidget(playlistPane);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);
    splitter->setStretchFactor(2, 1);

    QGridLayout *center_layout = new QGridLayout;
    center_layout->addWidget(search, 0, 0, Qt::AlignLeft);
    center_layout->addWidget(MiniMode, 0, 2, Qt::AlignRight);
    center_layout->addWidget(splitter, 1, 0, 1, -1);

    // Add all to main_layout
    QVBoxLayout *main_layout = new QVBoxLayout(widget);
    main_layout->addLayout(center_layout);
    main_layout->addWidget(play_controller);
    widget->setLayout(main_layout);
    this->setCentralWidget(widget);

    connect(MiniMode, SIGNAL(clicked()),
            this, SLOT(send_mini_mode()));
    connect(Media_Manager::get(), SIGNAL(center_changed(Collection*)),
            centerPane, SLOT(set_title(Collection*)));
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

QStringList* Main_View::get_files()
{
    QStringList *files = new QStringList(QFileDialog::getOpenFileNames(this, tr("Select Music Files"),
        QDesktopServices::storageLocation(QDesktopServices::MusicLocation)));

    return files;
}

void Main_View::create_playlist_files()
{
    QStringList *files = get_files();
    Media_Manager::get()->new_playlist(files);

    playlist->setModel(Media_Manager::get()->get_playlist());
}

void Main_View::add_files()
{
    //Single file
    QString *file = new QString(QFileDialog::getOpenFileName(this, tr("Select Music Files"),
        QDesktopServices::storageLocation(QDesktopServices::MusicLocation)));

    if (!file)
        return;

    qDebug() << *file << endl;
    Media_Manager::get()->play_new(*file);
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
    //iff its a playlist, we must have selected a song,
    //  so let's change the current playlist to that song
    if (Playlist *plist = dynamic_cast<Playlist*>(Media_Manager::get()->get_center()))
    {
        qDebug() << plist->get_name();
        Media_Manager::get()->switch_playlist(plist, index.row());

        playlist->setModel(plist);
    }

    // send whatever we selected into the center
    Media_Manager::get()->get_center()->select_child(index.row());

    table->setModel(Media_Manager::get()->get_center());

}

void Main_View::delete_playlist_from_library()
{
    QModelIndexList indexes = lib_list->selectionModel()->selection().indexes();
    for (int i = 0; i < indexes.count(); ++i)
    {
        Media_Manager::get()->get_library()->remove(indexes[i].row());
    }
}

void Main_View::remove_selected_song()
{
    QModelIndexList indexes = playlist->selectionModel()->selection().indexes();
    for (int i = 0; i < indexes.count(); ++i)
    {
        Media_Manager::get()->get_playlist()->remove(indexes[i].row());
    }

}
