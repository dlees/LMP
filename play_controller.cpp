#include <QVBoxLayout>
#include <QGridLayout>
#include "play_controller.h"
#include "button_container.h"
#include "playing_song.h"
#include <QSlider>
#include <vector>
#include <Phonon>
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <algorithm>

extern Phonon::AudioOutput *curAudio;

Play_Controller::Play_Controller()
    : button_container(new Button_Container(100, 30))
{
    QVBoxLayout *main_layout = new QVBoxLayout();
    slider = new QSlider(Qt::Horizontal);
    song_title = new QLabel;

    song_title->setMaximumWidth(200);

    HotSpots = new QLabel("Hotspots: None");

    Phonon::VolumeSlider *volumeSlider;
    volumeSlider = new Phonon::VolumeSlider(this);
    volumeSlider->setAudioOutput(curAudio);
    volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    volumeSlider->setMaximumWidth(200);

    QGridLayout *inner_layout = new QGridLayout();

    inner_layout->addWidget(song_title, 0, 0, Qt::AlignCenter);
    inner_layout->addWidget(button_container, 0, 1, Qt::AlignCenter);
    inner_layout->addWidget(volumeSlider, 0, 2, Qt::AlignCenter);

    main_layout->addWidget(HotSpots);
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
    connect(&Playing_Song::get(), SIGNAL(song_changed(Song *, int)),
            this, SLOT(set_new_song(Song *, int)));
    connect(&Playing_Song::get(), SIGNAL(hs_added()),
            this, SLOT(set_hs_text()));
}

void Play_Controller::set_slider_position(qint64 value)
{
    slider->setValue((int) value);
}

void Play_Controller::send_new_position()
{
    Playing_Song::get().change_position(slider->value());
}

void Play_Controller::set_new_song(Song *song, int time)
{
    //set total duration
    slider->setMaximum(time);
    qDebug() << "TIME: " << time;

    slider->setValue(0);
    song_title->setText(song->get_name());

    set_hs_text();
}

void Play_Controller::set_hs_text()
{
    std::vector<int> HS_list = Playing_Song::get().get_hs_list();
    std::sort(HS_list.begin(), HS_list.end());
    //HotSpots->setText("");

    qDebug() << "HS_list Size: " << HS_list.size();

    if (HS_list.size() == 0)
    {
        HotSpots->setText("Hotspots: None");
    }
    else
    {
        QString temp = "Hotspots: ";
        QString temp2;

        int miliSecs = HS_list[0];

        int secs = miliSecs/1000;
        int minutes = secs/60;

        temp2.setNum(minutes);
        temp.append(temp2);
        temp.append(":");
        temp2.setNum(secs%60);
        temp.append(temp2);
        for (int i=1; i<HS_list.size(); i++)
        {
            temp.append(", ");

            miliSecs = HS_list[i];
            secs = miliSecs/1000;
            minutes = secs/60;

            temp2.setNum(minutes);
            temp.append(temp2);
            temp.append(":");
            if (secs%60 < 10)
            {
                temp.append("0");
            }
            temp2.setNum(secs%60);
            temp.append(temp2);

        }

        HotSpots->setText(temp);
    }
}

Play_Controller::~Play_Controller()
{

}
