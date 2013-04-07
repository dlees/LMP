#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

/* This class wraps the way music is
 * actually played away from PlayingSong.
 *
 */

#include <QString>
#include <Phonon>
#include <QCoreApplication>

class Music_Player
{
public:
    Music_Player();

    // gets how many milliseconds are in the song
    int get_duration();

    // starts playing the song at the current position
    void play_song();

    // returns true if the current song cant be played
    bool cant_play();

    // pauses the current playing song
    void pause_song();

    // sets the song we will play to filename
    void set_song(QString filename);

    // stops playing the current song and prepares it to be replaced
    void remove_song();

    // seeks to new_pos in the cur playing song
    // throw new Error("Can't seek to that position");
    // don't check if past total length of song
    void move_to(int new_pos); // millisecs

    // changes the volume, needs to be Stringed in derived class
    void change_volume(int percent);

    // gets how loud the player is playing at (0-100)
    //	0 = no sound, 100 = max loudness
    int get_volume_percent();

private:
    Phonon::MediaObject *curSong;
    Phonon::AudioOutput *curAudio;
};

#endif // MUSIC_PLAYER_H
