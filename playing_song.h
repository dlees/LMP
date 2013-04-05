#ifndef PLAYING_SONG
#define PLAYING_SONG

#include <QObject>

#include "music_player.h"

class Song;

/* This class is used to record Second Counts
 *
 * There should only ever be one instance of it.
 *
 */

class Playing_Song : public QObject
{
    Q_OBJECT

public:
    Playing_Song();

    // changes the curSong to the song
    // does not play
    void replace_song(Song *song_);

    // throws an Error if cant play the song
    void play();

    void pause();


    // throws error if past total duration
    void change_position(int new_pos);
    void change_pos_relative(int relative_pos);

    // hotspots
    void set_hs();
    void next_hs();
    void prev_hs();
    void remove_hs();

    bool is_paused(); // true if paused

    bool is_finished();	// return true if the song ended

    // volume
    void vol_up();
    void vol_down();
    void set_volume_percent(int percent);
    int get_volume_percent();

    void close();

    Song *get_cur_song()
        { return cur_playing; }

signals:
    void became_paused();
    void started_playing();

private:
    Song *cur_playing;
    bool paused;	// true if the song is paused
    int totalDuration; // total length of the song in millisecs
    int position;	// position in the song
    qint64 start_time; // time the song started playing last
    Music_Player mp;

    // prints second count to the file
    void update_position();

    // This should be used to get the position, but not update count
    qint64 get_position();

    bool song_exists();

    void set_song(Song *song_);
};

#endif
