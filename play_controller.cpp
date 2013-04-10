#include <QVBoxLayout>

#include "play_controller.h"
#include "button_container.h"
#include "playing_song.h"
#include <QSlider>
#include <Phonon>
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>

extern Phonon::AudioOutput *curAudio;

Play_Controller::Play_Controller()
    : button_container(new Button_Container(100, 30))
{
    QVBoxLayout *main_layout = new QVBoxLayout();
    slider = new QSlider(Qt::Horizontal);

    Phonon::VolumeSlider *volumeSlider;
    volumeSlider = new Phonon::VolumeSlider(this);
    volumeSlider->setAudioOutput(curAudio);
    volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QHBoxLayout *inner_layout = new QHBoxLayout();

    inner_layout->addWidget(button_container);
    inner_layout->addWidget(volumeSlider);

    main_layout->addWidget(slider);
    main_layout->addLayout(inner_layout);

    setLayout(main_layout);
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(Playing_Song::get().get_media_object(), SIGNAL(tick(qint64)),
            this, SLOT(set_slider_position(qint64)));
    connect(slider, SIGNAL(sliderReleased()),
            this, SLOT(send_new_position()));
    connect(&Playing_Song::get(), SIGNAL(new_total_duration(int)),
            this, SLOT(set_total_value(int)));
}

void Play_Controller::set_slider_position(qint64 value)
{

    slider->setValue((int) value);

}

void Play_Controller::send_new_position()
{
    Playing_Song::get().change_position(slider->value());
}

void Play_Controller::set_total_value(int time)
{
    slider->setMaximum(time);
    qDebug() << "TIME: " << time;
}

Play_Controller::~Play_Controller()
{

}
