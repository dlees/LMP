#include "main_view.h"

#include <QtGui>

#include "stardelegate.h"
#include "media_manager.h"
#include "button_container.h"
#include "play_controller.h"
#include "pane.h"
#include "collection.h"
#include "playlist.h"
#include "error.h"
#include "database.h"

Main_View::Main_View(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;

    Play_Controller *play_controller = new Play_Controller();

    //QLineEdit *search = new QLineEdit("search");
    MiniModeButton = new QToolButton(this);
    MiniModeButton->setText("-");
    MiniModeButton->setToolTip("Switch to MiniMode");

    create_message_bar();

    // Splitter to hold the center panes
    splitter = new QSplitter(Qt::Horizontal);

    //2nd row containing Splitter of 3 panes
    create_library_pane();
    create_center_pane();
    create_playlist_pane();

    //QCustomPlot
//    create_graph();

    QGridLayout *center_layout = new QGridLayout;
    //center_layout->addWidget(search, 0, 0, Qt::AlignLeft);
    center_layout->addWidget(MiniModeButton, 0, 0, Qt::AlignLeft);
    center_layout->addWidget(message_bar, 0, 1, Qt::AlignLeft);
    center_layout->addWidget(splitter, 1, 0, 1, -1);

    // Add all to main_layout
    QVBoxLayout *main_layout = new QVBoxLayout(widget);
    main_layout->addLayout(center_layout);
    main_layout->addWidget(play_controller);
    widget->setLayout(main_layout);
    this->setCentralWidget(widget);

    connect(MiniModeButton, SIGNAL(clicked()),
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

void Main_View::create_message_bar()
{
    message_bar = new QLabel(this);

    message_bar->setText("NO Message");

    connect(&Playing_Song::get(), SIGNAL(message(QString)),
            message_bar, SLOT(setText(QString)));
}

void Main_View::create_library_pane()
{
    lib_list = new QListView();
    libraryPane = new Pane("Library", lib_list);
    lib_list->setModel(Media_Manager::get()->get_library());
    connect(lib_list, SIGNAL(clicked(QModelIndex)),
            this, SLOT(select_lib_item(QModelIndex)));
    connect(lib_list, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(open_playlist_from_lib(QModelIndex)));

    // Add to splitter
    splitter->addWidget(libraryPane);

    qDebug() << "Library Pane Created";
}

void Main_View::create_center_pane()
{
    table = new QTableView();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSortingEnabled(true);
    table->setShowGrid(false);
    table->setModel(Media_Manager::get()->get_playlist());
    table->setColumnWidth(0, 200);
    centerPane = new Pane("Default Playlist", table);
    connect(table, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select_center_item(QModelIndex)));

    table->setItemDelegate(new StarDelegate);

    // Add to splitter
    splitter->addWidget(centerPane);

    qDebug() << "Center Pane Created";
}

void Main_View::create_playlist_pane()
{
    playlist = new QListView();
    playlist->setModel(Media_Manager::get()->get_playlist());
    playlistPane = new Pane("Current Playlist: All Songs", playlist);
    connect(playlist, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select_song(QModelIndex)));
    playlist->setDragEnabled(true);
    playlist->viewport()->setAcceptDrops(true);
    playlist->setDropIndicatorShown(true);

    // Add to splitter
    splitter->addWidget(playlistPane);

    qDebug() << "Playlist Pane Created";
}

void Main_View::create_graph()
{
    QVector<double> x(101), y(101);
    for (int i = 0; i < 101; i++) {
        x[i] = i/50.0 -1;
        y[i] = x[i]*x[i];
    }
    customPlot = new QCustomPlot();
    customPlot->addGraph();
    customPlot->graph(0)->setData(x,y);
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
    customPlot->replot();

    // Add to splitter
    splitter->addWidget(customPlot);
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

void Main_View::add_files()
{
    //Single file
    QString *file = new QString(get_files()->at(0));

    if (!file->size()) {
        Error::print_error_msg_str("No file selected!");
        return;
    }

    qDebug() << *file;
    Media_Manager::get()->play_new(*file);
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


void Main_View::quit()
{
    Database::get()->save_songs();
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

void Main_View::remove_selected_from_table()
{
    QModelIndexList indexes = table->selectionModel()->selectedRows();

    qSort(indexes.begin(), indexes.end());

    for (int i = indexes.count()-1; i >= 0; --i)
    {
        Media_Manager::get()->get_center()->remove(indexes[i].row());
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

void Main_View::create_catalog()
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->resize(300, 200);

    QLineEdit *text = new QLineEdit(msgBox);
    text->setMinimumWidth(200);
    text->setAlignment(Qt::AlignHCenter);
    msgBox->setInformativeText("Catalog Name:");
    QHBoxLayout *layout = new QHBoxLayout(msgBox);
    layout->addWidget(text);

    msgBox->setLayout(layout);
    msgBox->setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox->setDefaultButton(QMessageBox::Ok);
    text->setFocus();
    int ret = msgBox->exec();

    if (ret == QMessageBox::Ok && text->text() != "")
        Media_Manager::get()->create_catalog(text->text(), Database::get());

    delete msgBox;
}

void Main_View::add_selected_lib_list_to_catalog()
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->resize(300, 200);

    QLineEdit *text = new QLineEdit(msgBox);
    text->setMinimumWidth(200);
    text->setAlignment(Qt::AlignHCenter);
    msgBox->setInformativeText("Catalog Name:");
    QHBoxLayout *layout = new QHBoxLayout(msgBox);
    layout->addWidget(text);

    msgBox->setLayout(layout);
    msgBox->setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox->setDefaultButton(QMessageBox::Ok);
    text->setFocus();
    int ret = msgBox->exec();

    if (ret == QMessageBox::Ok && text->text() != "")
    {
        QModelIndexList indexes = lib_list->selectionModel()->selection().indexes();
        for (int i = 0; i < indexes.count(); ++i)
        {
            Music_Item *item = Media_Manager::get()->get_library()->get_children()[indexes[i].row()];
            Media_Manager::get()->add_to_catalog(text->text(), item);
        }
    }
}

#include "datalist.h"
#include "datalistdecorator.h"
#include "dynamicplaylist.h"
void Main_View::create_auto_playlist()
{
    delete new DynamicPlaylist;
    /*
    DataList *datalist = Media_Manager::get()->get_center()->convert_to_secCount_datalist();

    try {
    datalist = create_decorator_combiner()
            ->build(filter_decorator("less than", 5000))
            ->build(sort_decorator("value"))
            ->build(create_playlist_decorator("Auto Playlist"))
        ->decorate(datalist);
    } catch (Error &e) {
        e.print_error_msg();
    }

    delete datalist;*/
}

void Main_View::create_top_albums_catalog()
{
    DataList *datalist = Media_Manager::get()->get_library()->convert_to_secCount_datalist();

    try {
    datalist = create_decorator_combiner()
            ->build(filter_decorator("less than", 10000)) //filter out ratings less than 3
            ->build(sort_decorator("value")) // sort by greatest rating
            ->build(create_playlist_decorator("Top Playlists", true))
        ->decorate(datalist);
    } catch (Error &e) {
        e.print_error_msg();
    }

    delete datalist;
}

void Main_View::add_good_to_table()
{
    DataList *datalist = Media_Manager::get()->get_playlist()->convert_to_rating_datalist();

    try {
    datalist = create_decorator_combiner()
            ->build(filter_decorator("less than", 3)) //filter out ratings less than 3
            ->build(sort_decorator("value")) // sort by greatest rating
            ->build(add_to_collection_decorator(Media_Manager::get()->get_center()))
        ->decorate(datalist);
    } catch (Error &e) {
        e.print_error_msg();
    }

    delete datalist;
}

void Main_View::export_running_total()
{
    DataList *datalist = Database::get()->get_sec_count_data();

    try {
    datalist = create_decorator_combiner()
            ->build(new OutputToFile("Database.txt"))
        ->decorate(datalist);
    } catch (Error &e) {
        e.print_error_msg();
    }
    delete datalist;
}

void Main_View::export_finite_difference()
{
    DataList *datalist = Media_Manager::get()->get_library()->convert_to_secCount_datalist();

    try {
    create_decorator_combiner()
            ->build(filter_decorator("less than", 10000)) //filter out ratings less than 3
            ->build(sort_decorator("value")) // sort by greatest rating
        ->decorate(datalist);
    } catch (Error &e) {
        e.print_error_msg();
    }
    delete datalist;
}

