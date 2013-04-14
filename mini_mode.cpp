#include "mini_mode.h"


#include <QtGui>

#include "media_manager.h"
#include "button_container.h"
#include "play_controller.h"
#include "pane.h"
#include "collection.h"
#include "mini_mode_controller.h"

Mini_Mode::Mini_Mode(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;

    Mini_Mode_Controller *play_controller = new Mini_Mode_Controller();

    MainMode = new QPushButton("M");

    QGridLayout *temp = new QGridLayout();
    temp->addWidget(play_controller, 0,0,5,10,Qt::AlignCenter);
    temp->addWidget(MainMode,0,10,1,1,Qt::AlignRight);


    widget->setLayout(temp);
    this->setCentralWidget(widget);

    connect(MainMode, SIGNAL(clicked()),
            this, SLOT(send_to_main()));
}

void Mini_Mode::create_menu()
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

QAction *Mini_Mode::add_menu_item(char name[], bool enabled)
{
    QAction *newAct = new QAction(tr(name), this);
    newAct->setEnabled(enabled);
    fileMenu->addAction(newAct);

    return newAct;
}

Mini_Mode::~Mini_Mode()
{

}

void Mini_Mode::play()
{
    Media_Manager::get()->play_cur();
}

void Mini_Mode::open_and_play()
{
    Media_Manager::get()->play_new("C:/Call Me Maybe");
}

void Mini_Mode::add_files()
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

void Mini_Mode::quit()
{
    QApplication::quit();
}

void Mini_Mode::send_to_main()
{
    emit to_main_mode();
}