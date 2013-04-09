#include "button_container.h"
#include "media_manager.h"
#include <QFrame>

Button_Container::Button_Container(int w, int h)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    playPause = new QToolButton(this);
    //playPause->setIcon(QIcon(":/buttons/pause.png"));
    playPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    playPause->setGeometry(0, 0, 10, 20);
    connect(playPause, SIGNAL(clicked()),
            Media_Manager::get(), SLOT(play_cur()));

    previous = new QToolButton(this);
    //previous->setIcon(QIcon(":/buttons/prev.png"));
    previous->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    previous->setGeometry(0, 0, 10, 20);
    connect(previous, SIGNAL(clicked()),
            Media_Manager::get(), SLOT(prev()));

    next = new QToolButton(this);
    //next->setIcon(QIcon(":/buttons/next.png"));
    next->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
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
    //playPause->setIcon(QIcon(QString::fromUtf8(":/buttons/play.png")));
    playPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

void Button_Container::set_icon_pause()
{
    //playPause->setIcon(QIcon(QString::fromUtf8(":/buttons/pause.png")));
    playPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
}

Button_Container::~Button_Container()
{
    delete playPause;
    delete previous;
    delete next;
}
