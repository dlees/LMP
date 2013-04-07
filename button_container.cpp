#include "button_container.h"
#include "media_manager.h"

Button_Container::Button_Container(int w, int h)
{
    //setFixedSize(w, h);

    playPause = new QToolButton(this);
    playPause->setIcon(QIcon(":/buttons/pause.png"));
    playPause->setGeometry(0, 0, 10, 20);
    connect(playPause, SIGNAL(clicked()),
            Media_Manager::get(), SLOT(play_cur()));

    previous = new QToolButton(this);
    previous->setIcon(QIcon(":/buttons/prev.png"));
    previous->setGeometry(0, 0, 10, 20);
    connect(previous, SIGNAL(clicked()),
            Media_Manager::get(), SLOT(prev()));

    next = new QToolButton(this);
    next->setIcon(QIcon(":/buttons/next.png"));
    next->setGeometry(0, 0, 10, 20);
    connect(next, SIGNAL(clicked()),
            Media_Manager::get(), SLOT(next()));

    connect(Media_Manager::get(), SIGNAL(became_paused()),
             this, SLOT(set_icon_play()));
    connect(Media_Manager::get(), SIGNAL(started_playing()),
             this, SLOT(set_icon_pause()));

    addWidget(previous);
    addWidget(playPause);
    addWidget(next);
}

void Button_Container::set_icon_play()
{
    playPause->setIcon(QIcon(QString::fromUtf8(":/buttons/play.png")));
}

void Button_Container::set_icon_pause()
{
    playPause->setIcon(QIcon(QString::fromUtf8(":/buttons/pause.png")));
}

Button_Container::~Button_Container()
{
    delete playPause;
    delete previous;
    delete next;
}
