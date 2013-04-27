#include "main_view.h"

#include <QtGui>

#include "media_manager.h"
#include "button_container.h"
#include "play_controller.h"
#include "pane.h"
#include "collection.h"
#include "playlist.h"
#include "error.h"

Main_View::Main_View(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;

    Play_Controller *play_controller = new Play_Controller();

    // 1st row containing "Playlist name", searchbar, minimode button
    //QLineEdit *search = new QLineEdit("search");
    MiniMode = new QToolButton(this);
    MiniMode->setText("-");
    MiniMode->setToolTip("Switch to MiniMode");

    //2nd row containing Splitter of 3 panes
// Library Pane
    lib_list = new QListView();
    Pane *libraryPane = new Pane("Library", lib_list);
    lib_list->setModel(Media_Manager::get()->get_library());
    connect(lib_list, SIGNAL(clicked(QModelIndex)),
            this, SLOT(select_lib_item(QModelIndex)));
    connect(lib_list, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(open_playlist_from_lib(QModelIndex)));

// Center Table
    table = new QTableView();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSortingEnabled(true);
    table->setShowGrid(false);
    table->setModel(Media_Manager::get()->get_playlist());
    table->setColumnWidth(0, 200);
    centerPane = new Pane("Default Playlist", table);
    connect(table, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select_center_item(QModelIndex)));

// Playlist Pane
    playlist = new QListView();
    playlist->setModel(Media_Manager::get()->get_playlist());
    playlistPane = new Pane("Current Playlist: All Songs", playlist);
    connect(playlist, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select_song(QModelIndex)));
    playlist->setDragEnabled(true);
    playlist->viewport()->setAcceptDrops(true);
    playlist->setDropIndicatorShown(true);

    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(libraryPane);
    splitter->addWidget(centerPane);
    splitter->addWidget(playlistPane);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);
    splitter->setStretchFactor(2, 1);

    QGridLayout *center_layout = new QGridLayout;
    //center_layout->addWidget(search, 0, 0, Qt::AlignLeft);
    center_layout->addWidget(MiniMode, 0, 0, Qt::AlignLeft);
    center_layout->addWidget(splitter, 1, 0, 1, -1);

    // Add all to main_layout
    QVBoxLayout *main_layout = new QVBoxLayout(widget);
    main_layout->addLayout(center_layout);
    main_layout->addWidget(play_controller);
    widget->setLayout(main_layout);
    this->setCentralWidget(widget);

    connect(MiniMode, SIGNAL(clicked()),
            this, SLOT(send_mini_mode()));

    // Set the title of the center pane
    //connect(Media_Manager::get(), SIGNAL(center_changed(Collection*)),
      //      centerPane, SLOT(set_title(Collection*)));

    // tell table that the center changed
    connect(Media_Manager::get(), SIGNAL(center_changed(Collection*)),
            this, SLOT(update_center(Collection*)));

    // tell playlist of change
    connect(Media_Manager::get(), SIGNAL(playlist_changed(Collection*)),
            this, SLOT(update_playlist(Collection *)));

    lib_list->setCurrentIndex(Media_Manager::get()->get_library()->index(0,0));

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
    QString name;

    QInputDialog *getName = new QInputDialog();

    getName->setCancelButtonText("Cancel");
    getName->setOkButtonText("OK");
    getName->setLabelText("Playlist Name: ");
    getName->setInputMode(QInputDialog::TextInput);

    getName->show();

    connect(getName, SIGNAL(textValueSelected(QString)),
            this, SLOT(new_create_playlist_files(QString)));
}

void Main_View::new_create_playlist_files(QString name)
{
    QStringList *files = get_files();

    if (!files->size()) {
        Error::print_error_msg_str("No file selected!");
        return;
    }

    Media_Manager::get()->new_playlist(files, name);

    playlist->setModel(Media_Manager::get()->get_playlist());

    lib_list->setCurrentIndex(Media_Manager::get()->get_library()->index(
                                  Media_Manager::get()->get_library()->count()-1,0));
}

void Main_View::add_files()
{
    //Single file
    QString *file = new QString(QFileDialog::getOpenFileName(this, tr("Select Music Files"),
        QDesktopServices::storageLocation(QDesktopServices::MusicLocation)));

    if (!file->size()) {
        Error::print_error_msg_str("No file selected!");
        return;
    }

    qDebug() << *file << endl;
    Media_Manager::get()->play_new(*file);
}

void Main_View::quit()
{
    QApplication::quit();
}

void Main_View::send_mini_mode()
{
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

void Main_View::open_playlist_from_lib(QModelIndex index)
{
    // start playing any thing in this index
    Media_Manager::get()->get_library()->select_child(index.row());

    Media_Manager::get()->get_center()->begin_playing();

    playlist->setModel(Media_Manager::get()->get_playlist());
}

void Main_View::select_center_item(QModelIndex index)
{
    //iff its a playlist, we must have selected a song,
    //  so let's change the current playlist to that song
    if (Playlist *plist = dynamic_cast<Playlist*>(Media_Manager::get()->get_center()))
    {
        Media_Manager::get()->switch_playlist(plist, index.row());

        playlist->setModel(plist);
    }

    // send whatever we selected into the center
    Media_Manager::get()->get_center()->select_child(index.row());
}

void Main_View::delete_playlist_from_library()
{
    QModelIndexList indexes = lib_list->selectionModel()->selection().indexes();
    for (int i = 0; i < indexes.count(); ++i)
    {
        // if this playlist is currently playing
       if (Media_Manager::get()->get_library()->get_children()[indexes[i].row()]->get_is_playing())
       {
            Error::print_error_msg_str("Can't delete current playing playlist.");
            continue;
       }

        // if this collection is in the table currently
        if (Media_Manager::get()->get_library()->get_children()[indexes[i].row()] == Media_Manager::get()->get_center())
        {
            // set All Songs to be in the center
            Media_Manager::get()->get_library()->select_child(0);

            // select all songs in the lib_list
            lib_list->setCurrentIndex(Media_Manager::get()->get_library()->index(0,0));
        }
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

void Main_View::add_selected_to_playlist()
{
    QModelIndexList indexes = table->selectionModel()->selectedRows();
    for (int i = 0; i < indexes.count(); ++i)
    {
        Music_Item *item = Media_Manager::get()->get_center()->get_children()[indexes[i].row()];
        Media_Manager::get()->get_playlist()->add(item);
    }
}

void Main_View::update_center(Collection *collection)
{
    table->setModel(Media_Manager::get()->get_center());
    centerPane->set_title(collection->get_name());
}

void Main_View::update_playlist(Collection *collection)
{
    playlist->setModel(Media_Manager::get()->get_playlist());
    playlistPane->set_title("Current Playlist: " +collection->get_name());
}
