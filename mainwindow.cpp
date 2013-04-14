#include "mainwindow.h"

#include <QtGui>

#include "media_manager.h"
#include "button_container.h"
#include "play_controller.h"
#include "pane.h"
#include "collection.h"
#include "main_view.h"
#include "mini_mode.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    mainView = new Main_View();
    miniView = new Mini_Mode();

    //miniView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    miniView->setMaximumHeight(100);
    miniView->setMaximumWidth(300);

    //miniSize.setWidth(225);
    //miniSize.setHeight(75);
    miniSize.setWidth(225);
    miniSize.setHeight(100);

    create_menu();

    this->setCentralWidget(mainView);

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

    editMenu = menuBar()->addMenu(tr("&Edit"));

    hsMenu = menuBar()->addMenu(tr("&Hotspots"));
    connect(add_menu_item((char*)"Add hotspot", true, hsMenu), SIGNAL(triggered()),
            &Playing_Song::get(), SLOT(set_hs()));

    plMenu = menuBar()->addMenu(tr("&Playlists"));
    connect(add_menu_item((char*)"Create Empty Playlist", true, plMenu), SIGNAL(triggered()),
            Media_Manager::get(), SLOT(new_playlist()));
    connect(add_menu_item((char*)"Create Playlist From Files...", true, plMenu), SIGNAL(triggered()),
            Media_Manager::get(), SLOT());
    connect(add_menu_item((char*)"Add song to current playlist", true, plMenu), SIGNAL(triggered()),
            Media_Manager::get(), SLOT(add_cur_to_playlist()));

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
        qDebug() << filename << endl;
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

}

void MainWindow::switch_view_to_main()
{
    qDebug() << "SWITCH_VIEW_TO_MAIN";

    //miniSize = this->size();

    miniView->setParent(0);

    this->setCentralWidget(mainView);

    this->resize(mainSize);
}
