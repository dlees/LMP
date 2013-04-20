#include "button_container.h"
#include "media_manager.h"
#include <QFrame>

Button_Container::Button_Container(int w, int h)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    playPause = new QToolButton(this);
    //playPause->setIcon(QIcon(":/buttons/pause.png"));
    //playPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    set_icon_play();
    playPause->setGeometry(0, 0, 10, 20);
    connect(playPause, SIGNAL(clicked()),
            Media_Manager::get(), SLOT(play_cur()));

    prev = new QToolButton(this);
    //prev->setIcon(QIcon(":/buttons/prev.png"));
    prev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    prev->setGeometry(0, 0, 10, 20);
    connect(prev, SIGNAL(clicked()),
            Media_Manager::get(), SLOT(prev()));
    prev->setToolTip("Skip to Previous Song");

    next = new QToolButton(this);
    //next->setIcon(QIcon(":/buttons/next.png"));
    next->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    next->setGeometry(0, 0, 10, 20);
    connect(next, SIGNAL(clicked()),
            Media_Manager::get(), SLOT(next()));
    next->setToolTip("Skipt to Next Song");

    hotspot_prev = new QToolButton(this);
    hotspot_prev->setIcon(QIcon(":/buttons/hotspot_prev.png"));
    //next->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    hotspot_prev->setGeometry(0, 0, 10, 20);
    connect(hotspot_prev, SIGNAL(clicked()),
            this, SLOT(prev_hs()));
    hotspot_prev->setToolTip("Move to a previous Hotspot!");

    hotspot_next = new QToolButton(this);
    hotspot_next->setIcon(QIcon(":/buttons/hotspot_next.png"));
    //hotspot_next->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    hotspot_next->setGeometry(0, 0, 10, 20);
    connect(hotspot_next, SIGNAL(clicked()),
            this, SLOT(next_hs()));
    hotspot_next->setToolTip("Move to the next Hotspot!");

    connect(Media_Manager::get(), SIGNAL(became_paused()),
             this, SLOT(set_icon_play()));
    connect(Media_Manager::get(), SIGNAL(started_playing()),
             this, SLOT(set_icon_pause()));

    addWidget(prev);
    addWidget(hotspot_prev);
    addWidget(playPause);
    addWidget(hotspot_next);
    addWidget(next);
}

void Button_Container::set_icon_play()
{
    //playPause->setIcon(QIcon(":/buttons/play.png"));
    playPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

void Button_Container::set_icon_pause()
{
    //playPause->setIcon(QIcon(":/buttons/pause.png"));
    playPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
}

void Button_Container::prev_hs()
{
    Playing_Song::get().prev_hs();
}

void Button_Container::next_hs()
{
    Playing_Song::get().next_hs();
}

Button_Container::~Button_Container()
{
    delete playPause;
    delete prev;
    delete next;
}
