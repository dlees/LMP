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
void Main_View::create_playlist_date_range()
{

    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText("Enter time range");
    msgBox->resize(300, 600);

    QGridLayout *layout = qobject_cast<QGridLayout *>(msgBox->layout());

    QDateTimeEdit *start_date_w = new QDateTimeEdit(msgBox);
    start_date_w->setAlignment(Qt::AlignHCenter);
    start_date_w->setCalendarPopup(true);
    start_date_w->setDateTime(QDateTime::currentDateTime().addDays(-1));
    layout->addWidget(start_date_w, 1, 0);

    QDateTimeEdit *end_date_w = new QDateTimeEdit(msgBox);
    end_date_w->setAlignment(Qt::AlignHCenter);
    end_date_w->setCalendarPopup(true);
    end_date_w->setDateTime(QDateTime::currentDateTime());
    layout->addWidget(end_date_w, 1, 1);

    msgBox->setLayout(layout);
    msgBox->setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox->setDefaultButton(QMessageBox::Ok);
    start_date_w->setFocus();

    int ret = msgBox->exec();

    if (ret == QMessageBox::Ok)
    {
        time_t start_date = start_date_w->dateTime().toTime_t();
        time_t end_date = end_date_w->dateTime().toTime_t();

        Media_Manager::get()->add_playlist_to_library(new DynamicPlaylist(start_date, end_date));
    }
    delete msgBox;

}

void Main_View::create_recent_playlist()
{
    time_t start_date = QDateTime::currentDateTime().addDays(-1).toTime_t();
    time_t end_date = QDateTime::currentDateTime().toTime_t();
    Media_Manager::get()->add_playlist_to_library(new DynamicPlaylist(start_date, end_date));
}

void Main_View::create_top_albums_catalog()
{
    DataList *datalist = Media_Manager::get()->get_library()->convert_to_secCount_datalist();

    try {
    datalist = create_decorator_combiner()
            ->build(filter_decorator("less than", 10000)) //filter out ratings less than 3
            ->build(sort_decorator("value")) // sort by greatest sec count
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
    static QMutex sec_count_mutex;

    sec_count_mutex.lock();
    QtConcurrent::run(this, &Main_View::export_complete_running_total);
    sec_count_mutex.unlock();
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

void Main_View::test1()
{
    DataList *datalist = Database::get()->get_sec_count_data();

    try {
    datalist = create_decorator_combiner()
            ->build(new FilterByTime(QDateTime::currentDateTime().addDays(-1).toTime_t(), QDateTime::currentDateTime().toTime_t()))
            ->build(new FilterByID(Media_Manager::get()->get_center()->convert_to_rating_datalist()))
                       ->build(new RunningTotal()) ->build(new GetNamesFromIDs)
            ->build(new LogDatalist())
        ->decorate(datalist);
    } catch (Error &e) {
        e.print_error_msg();
    }
    delete datalist;
}


void Main_View::export_running_total_by_song()
{
    DataList *datalist = Database::get()->get_sec_count_data();

    try {
    datalist = create_decorator_combiner()
            ->build(new FilterByTime(QDateTime::currentDateTime().addDays(-30).toTime_t(), QDateTime::currentDateTime().toTime_t()))
            ->build(new RunningTotal())
            ->build(new GetNamesFromIDs)
            ->build(new ExportToExcel("RunningTotalBySongExcel.txt"))
        ->decorate(datalist);

    message_bar->setText("Exported RunningTotalBySongExcel");

    } catch (Error &e) {
        e.print_error_msg();
    }
    delete datalist;
}

void Main_View::export_complete_running_total()
{
    DataList *datalist = Database::get()->get_sec_count_data();

    try {
    datalist = create_decorator_combiner()
            ->build(new MergeIDToOne())
            ->build(new RunningTotal())
            ->build(new SplitByID)
            ->build(new ExportToExcelSplit("RunningTotalExcel.txt"))
        ->decorate(datalist);

    message_bar->setText("Exported RunningTotalExcel");

    } catch (Error &e) {
        e.print_error_msg();
    }
    delete datalist;
}

void Main_View::export_playcount_data()
{
    DataList *datalist = Database::get()->get_sec_count_data();

     try {
     datalist = create_decorator_combiner()
             ->build(new GetNamesFromIDs)
           //  ->build(new FilterByTime(QDateTime::currentDateTime().addDays(-300).toTime_t(), QDateTime::currentDateTime().toTime_t()))
             ->build(new FilterByID(
                             create_decorator_combiner()
                             ->decorate(
                                 Media_Manager::get()->get_center()->convert_to_secCount_datalist()
                             )
                         ))
             ->build(new SplitByID)
             ->build(new SplitDatalistDecorator(
                         create_decorator_combiner()
                         ->build(new PlayCountCalculator(1000))
                         ))
             ->build(new ExportToExcelSplit("PlayCount.txt"))
         ->decorate(datalist);

     message_bar->setText("Exported PlayCount");
    } catch (Error &e) {
        e.print_error_msg();
    }
    delete datalist;
}

void Main_View::export_playcount_data_async()
{
    static QMutex play_count_mutex;

    play_count_mutex.lock();
    QtConcurrent::run(this, &Main_View::export_playcount_data);
    play_count_mutex.unlock();
}

#include <fstream>
#include <iostream>

DataList *create_datalist_from_android_seccounts() {
    std::string filename = "E:\\Downloads\\secCount.txt";
    std::ifstream fin(filename);

    DataList *datalist = DataList::get_instance();

    std::map<std::string, int> nameToId;
    int max_id = 0;

    for( std::string line; getline( fin, line ); ) // Get's the name or eof
    {
        std::string name = line;

        if (nameToId.find(name) == nameToId.end()) {
            nameToId[name] = max_id++;
        }

        int id = nameToId[name];

        getline( fin, line );
        int start_count = atoi(line.c_str());

        getline( fin, line );
        int end_count = atoi(line.c_str());

        getline( fin, line ); // First time is start time. we are used to seeing end time in other DBs
        getline( fin, line );
        time_t time = QDateTime::fromString(QString::fromStdString(line)).toTime_t();

        getline( fin, line );  // get the "==="

        datalist->add_data_point(DataPoint::get_instance(name, id, DataValue::get_instance(start_count, end_count), time));
    }

    return datalist;
}

void Main_View::test2()
{
    //export_playcount_data_async();
    DataList *datalist = create_datalist_from_android_seccounts();

    try {
    datalist = create_decorator_combiner()
            ->build(new TotalUp())
            ->build(sort_decorator("value"))
            ->build(new FilterByValue("less than", 1000))
            ->build(new LogDatalist)
        ->decorate(datalist);

    message_bar->setText("Exported PlayCount");
   } catch (Error &e) {
       e.print_error_msg();
   }
   delete datalist;

}

