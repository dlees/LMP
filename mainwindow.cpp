#include "mainwindow.h"

#include <QtGui>

#include "media_manager.h"
#include "playing_song.h"
#include "button_container.h"
#include "play_controller.h"
#include "pane.h"
#include "collection.h"
#include "main_view.h"
#include "mini_mode.h"
#include "database.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // wont work in windows :(
    //QCoreApplication::setAttribute(AA_CaptureMultimediaKeys, true);

    mainView = new Main_View();
    miniView = new Mini_Mode();

    miniView->setMaximumHeight(100);
    miniView->setMaximumWidth(500);

    mainView->setMinimumWidth(500);
    mainView->setMinimumHeight(400);

    // How big miniMode is
    miniSize.setWidth(230);
    miniSize.setHeight(60);

    create_menu();

    this->setCentralWidget(mainView);
    setWindowTitle("Lucidity Music Player");
    setWindowIcon(QIcon(":/buttons/headphone.png"));
    Media_Manager::get()->start_up();

    this->setMinimumHeight(49);
    this->setMinimumWidth(99);

    connect(mainView, SIGNAL(to_mini_mode()),
            this, SLOT(switch_view_to_mini()));
    connect(miniView, SIGNAL(to_main_mode()),
            this, SLOT(switch_view_to_main()));


}

void MainWindow::create_menu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    connect(add_menu_item((char*)"&Open song", true, fileMenu), SIGNAL(triggered()),
            mainView, SLOT(add_files()));
    connect(add_menu_item((char*)"&Exit", true, fileMenu), SIGNAL(triggered()),
            mainView, SLOT(quit()));

    viewMenu = menuBar()->addMenu(tr("&View"));
    connect(add_menu_item((char*)"Mini", true, viewMenu), SIGNAL(triggered()),
            this, SLOT(switch_view_to_mini()));
    connect(add_menu_item((char*)"Main", true, viewMenu), SIGNAL(triggered()),
            this, SLOT(switch_view_to_main()));

 // Hotspots Menu
    hsMenu = menuBar()->addMenu(tr("&Hotspots"));
    connect(add_menu_item((char*)"Add hotspot", true, hsMenu), SIGNAL(triggered()),
            &Playing_Song::get(), SLOT(set_hs()));
    connect(add_menu_item((char*)"Delete next hotspot", true, hsMenu), SIGNAL(triggered()),
            &Playing_Song::get(), SLOT(remove_next_hs()));

 // Playlist Menu
    plMenu = menuBar()->addMenu(tr("&Playlists"));
    connect(add_menu_item((char*)"Create Empty Playlist", true, plMenu), SIGNAL(triggered()),
            Media_Manager::get(), SLOT(new_playlist()));
    connect(add_menu_item((char*)"Create Playlist From Files...", true, plMenu), SIGNAL(triggered()),
            mainView, SLOT(create_playlist_files()));
    connect(add_menu_item((char*)"Create Auto Playlist", true, plMenu), SIGNAL(triggered()),
            mainView, SLOT(create_auto_playlist()));

    plMenu->addSeparator();
    connect(add_menu_item((char*)"Delete Selected Playlist in Library", true, plMenu), SIGNAL(triggered()),
            mainView, SLOT(delete_playlist_from_library()));

    plMenu->addSeparator();
    connect(add_menu_item((char*)"Add current song to table playlist", true, plMenu), SIGNAL(triggered()),
            Media_Manager::get(), SLOT(add_cur_to_playlist()));
    connect(add_menu_item((char*)"Add selected songs in table to current playlist", true, plMenu), SIGNAL(triggered()),
            mainView, SLOT(add_selected_to_playlist()));
    connect(add_menu_item((char*)"Add good songs in Cur List to Table List", true, plMenu), SIGNAL(triggered()),
            mainView, SLOT(add_good_to_table()));

    plMenu->addSeparator();
    connect(add_menu_item((char*)"Remove selected song from current playlist", true, plMenu), SIGNAL(triggered()),
            mainView, SLOT(remove_selected_song()));
    connect(add_menu_item((char*)"Remove selected items from table playlist", true, plMenu), SIGNAL(triggered()),
            mainView, SLOT(remove_selected_from_table()));

    plMenu->addSeparator();
    connect(add_menu_item((char*)"Move current song up", true, plMenu), SIGNAL(triggered()),
            Media_Manager::get(), SLOT(move_cur_song_up()));

 // Catalog Menu
    QMenu *catalogMenu = menuBar()->addMenu(tr("&Catalogs"));
    connect(add_menu_item((char*)"Create Catalog...", true, catalogMenu), SIGNAL(triggered()),
            mainView, SLOT(create_catalog()));
    connect(add_menu_item((char*)"Add selected Playlist to a Catalog...", true, catalogMenu), SIGNAL(triggered()),
            mainView, SLOT(add_selected_lib_list_to_catalog()));
    connect(add_menu_item((char*)"Create Top Albums Catalog", true, catalogMenu), SIGNAL(triggered()),
            mainView, SLOT(create_top_albums_catalog()));

 // Ratings Menu
    QMenu *ratingsMenu = menuBar()->addMenu(tr("&Ratings"));

    QSignalMapper *ratingsMapper = new QSignalMapper(this);
    QAction *rating1 = add_menu_item((char*)"Rate Current Song: 1 Star", true, ratingsMenu);
    QAction *rating2 = add_menu_item((char*)"Rate Current Song: 2 Stars", true, ratingsMenu);
    QAction *rating3 = add_menu_item((char*)"Rate Current Song: 3 Stars", true, ratingsMenu);
    QAction *rating4 = add_menu_item((char*)"Rate Current Song: 4 Stars", true, ratingsMenu);
    QAction *rating5 = add_menu_item((char*)"Rate Current Song: 5 Stars", true, ratingsMenu);
    QAction *rating6 = add_menu_item((char*)"Rate Current Song: SuperStared", true, ratingsMenu);

    connect(rating1 , SIGNAL(triggered()), ratingsMapper, SLOT(map()));
    connect(rating2 , SIGNAL(triggered()), ratingsMapper, SLOT(map()));
    connect(rating3 , SIGNAL(triggered()), ratingsMapper, SLOT(map()));
    connect(rating4 , SIGNAL(triggered()), ratingsMapper, SLOT(map()));
    connect(rating5 , SIGNAL(triggered()), ratingsMapper, SLOT(map()));
    connect(rating6 , SIGNAL(triggered()), ratingsMapper, SLOT(map()));

    ratingsMapper->setMapping(rating1, 1);
    ratingsMapper->setMapping(rating2, 2);
    ratingsMapper->setMapping(rating3, 3);
    ratingsMapper->setMapping(rating4, 4);
    ratingsMapper->setMapping(rating5, 5);
    ratingsMapper->setMapping(rating6, 6);

    connect(ratingsMapper, SIGNAL(mapped(int)), Media_Manager::get(), SLOT(change_rating_cur_song(int)));

}

QAction *MainWindow::add_menu_item(char name[], bool enabled, QMenu *menu)
{
    QAction *newAct = new QAction(tr(name), this);
    newAct->setEnabled(enabled);
    menu->addAction(newAct);

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

void MainWindow::add_files()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Music Files"),
        QDesktopServices::storageLocation(QDesktopServices::MusicLocation));

    if (files.isEmpty())
        return;

    foreach (QString filename, files)
    {
        qDebug() << filename;
    }
    Media_Manager::get()->play_new(files.at(0));
}

void MainWindow::quit()
{
    QApplication::quit();
}

void MainWindow::switch_view_to_mini()
{
    qDebug() << "SWITCH_VIEW_TO_MINI";

    mainSize = this->size();

    mainView->setParent(0);

    this->setCentralWidget(miniView);

    this->resize(miniSize);

    this->setWindowTitle("LMP");
    this->menuBar()->hide();
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->show();
}

void MainWindow::switch_view_to_main()
{
    qDebug() << "SWITCH_VIEW_TO_MAIN";

    //miniSize = this->size();

    miniView->setParent(0);

    this->setCentralWidget(mainView);

    this->resize(mainSize);
    this->setWindowTitle("Lucidity Music Player");
    this->menuBar()->show();
    this->setWindowFlags(0);
    this->show();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Z || event->key() == Qt::Key_F3)
    {
        Playing_Song::get().prev_hs();
    }
    else if (event->key() == Qt::Key_X || event->key() == Qt::Key_F4)
    {
        Playing_Song::get().next_hs();
    }
    else if (event->key() == Qt::Key_C || event->key() == Qt::Key_F5)
    {
        Playing_Song::get().set_hs();
    }
    else if (event->key() == Qt::Key_V || event->key() == Qt::Key_F6)
    {
        Playing_Song::get().remove_next_hs();
    }
    else if (event->key() == Qt::Key_F || event->key() == Qt::Key_F7)
    {
        Media_Manager::get()->first();
    }
    else if (event->key() == Qt::Key_R || event->key() == Qt::Key_F8)
    {
        Playing_Song::get().change_position(0);
    }
    else if (event->key() == Qt::Key_1)
    {
        Media_Manager::get()->change_rating_cur_song(1);
    }
    else if (event->key() == Qt::Key_2)
    {
        Media_Manager::get()->change_rating_cur_song(2);
    }
    else if (event->key() == Qt::Key_3 || event->key() == Qt::Key_F9)
    {
        Media_Manager::get()->change_rating_cur_song(3);
    }
    else if (event->key() == Qt::Key_4 || event->key() == Qt::Key_F10)
    {
        Media_Manager::get()->change_rating_cur_song(4);
    }
    else if (event->key() == Qt::Key_5 || event->key() == Qt::Key_F11)
    {
        Media_Manager::get()->change_rating_cur_song(5);
    }
    else if (event->key() == Qt::Key_6 || event->key() == Qt::Key_F12)
    {
        Media_Manager::get()->change_rating_cur_song(6);
    }
    //Glitchy
//    else if (event->key() == Qt::Key_A)
//    {
//        Playing_Song::get().change_pos_relative(-5);
//    }
//    else if (event->key() == Qt::Key_S)
//    {
//        Playing_Song::get().change_pos_relative(5);
//    }

    else if (event->key() == Qt::Key_MediaPlay)
    {
        Media_Manager::get()->play_cur();
    }
    else if (event->key() == Qt::Key_MediaStop || event->key() == Qt::Key_Escape)
    {
        if (centralWidget() == mainView)
            switch_view_to_mini();
        else
            switch_view_to_main();
    }
    else if (event->key() == Qt::Key_MediaNext || event->key() == Qt::Key_F2)
    {
        Media_Manager::get()->next();
    }
    else if (event->key() == Qt::Key_MediaPrevious || event->key() == Qt::Key_F1)
    {
        Media_Manager::get()->prev();
    }


}
