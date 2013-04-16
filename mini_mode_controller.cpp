#include "mini_mode_controller.h"
#include <QVBoxLayout>
#include "button_container.h"
#include "playing_song.h"
#include <QSlider>
#include <Phonon>
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>

extern Phonon::AudioOutput *curAudio;

Mini_Mode_Controller::Mini_Mode_Controller()
    : button_container(new Button_Container(100, 30))
{
    QVBoxLayout *main_layout = new QVBoxLayout();
    slider = new QSlider(Qt::Horizontal);

    QHBoxLayout *inner_layout = new QHBoxLayout();

    inner_layout->addWidget(button_container);

    main_layout->addLayout(inner_layout);

    setLayout(main_layout);
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
/*
    connect(Playing_Song::get().get_media_object(), SIGNAL(tick(qint64)),
            this, SLOT(set_slider_position(qint64)));
    connect(slider, SIGNAL(sliderReleased()),
            this, SLOT(send_new_position()));
    connect(&Playing_Song::get(), SIGNAL(new_total_duration(int)),
            this, SLOT(set_total_value(int)));
    connect(&Playing_Song::get(), SIGNAL(song_changed(Song *, int)),
            this, SLOT(set_total_value(Song *, int)));
*/
}
/*
void Mini_Mode_Controller::set_slider_position(qint64 value)
{

    slider->setValue((int) value);

}

void Mini_Mode_Controller::send_new_position()
{
    Playing_Song::get().change_position(slider->value());
}

void Mini_Mode_Controller::set_total_value(int time)
{
    slider->setMaximum(time);
    qDebug() << "TIME: " << time;
}
*/
Mini_Mode_Controller::~Mini_Mode_Controller()
{

}
