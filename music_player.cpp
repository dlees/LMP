#include "music_player.h"

#include <QDebug>

Phonon::AudioOutput *curAudio;

Music_Player::Music_Player()
{
    curSong = new Phonon::MediaObject();
    curAudio = new Phonon::AudioOutput(Phonon::MusicCategory);
    volumeSlider = new Phonon::VolumeSlider();

    Phonon::createPath(curSong, curAudio);

    curSong->setTickInterval(1000);
    volumeSlider->setAudioOutput(curAudio);
    volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

}

// gets how many milliseconds are in the song
int Music_Player::get_duration()
{
   return curSong->totalTime();
}

// starts playing the song that is in the player
// at the current position of the song
void Music_Player::play_song()
{
    qDebug() << "Playing...";

    curSong->play();

}

// returns true if the current song cant be played
bool Music_Player::cant_play()
{

    if (curSong->currentSource().type() == -1)
    {
        return true;
    }
    return false;
}

// pauses the current playing song
void Music_Player::pause_song()
{
    qDebug() << "Song Paused";

    curSong->pause();
}

// sets the song we will play to filename
void Music_Player::set_song(QString filename)
{
    qDebug() << "Song Changed to" << filename;

    curSong->setCurrentSource(filename);

    qDebug() << "     " << curSong->currentSource().fileName();
}

// stops playing the current song and prepares it to be replaced
void Music_Player::remove_song()
{
    qDebug() << "Song Stopped" ;

    curSong->stop();

}

// seeks to new_pos in the cur playing song
// throw new Error("Can't seek to that position"){}
// don't check if past total length of song
void Music_Player::move_to(int new_pos)
{
    // new_pos is in milliseconds
    qDebug() << "Seek to " << new_pos;

    curSong->seek(new_pos);

}

// changes the volume, needs to be Stringed in derived class
void Music_Player::change_volume(int percent)
{
    qDebug() << "Volume Changed to "<< percent ;
}

// gets how loud the player is playing at (0-100)
//	0 = no sound, 100 = max loudness
int Music_Player::get_volume_percent()
{
    return 100;
}

Phonon::MediaObject *::Music_Player:: get_media_object()
{
    return curSong;
}


Music_Player::~Music_Player()
{
    delete curSong;
    delete curAudio;
    delete volumeSlider;
}
