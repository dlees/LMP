#include <QVBoxLayout>

#include "play_controller.h"
#include "button_container.h"
#include "playing_song.h"
#include <QSlider>
#include <Phonon>

extern Phonon::AudioOutput *curAudio;

Play_Controller::Play_Controller()
    : button_container(new Button_Container(100, 30))
{
    QVBoxLayout *layout = new QVBoxLayout();
    slider = new QSlider(Qt::Horizontal);
    Phonon::VolumeSlider *volumeSlider;
    volumeSlider = new Phonon::VolumeSlider(this);
    volumeSlider->setAudioOutput(curAudio);
    volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    layout->addWidget(slider);
    layout->addWidget(button_container);
    layout->addWidget(volumeSlider);
    setLayout(layout);
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);

    connect(Playing_Song::get().get_media_object(), SIGNAL(tick(qint64)),
            this, SLOT(set_slider_position(qint64)));
}

void Play_Controller::set_slider_position(qint64 value)
{
    value = (value)/1000;
    slider->setValue((int) value);
}

Play_Controller::~Play_Controller()
{

}
