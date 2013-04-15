#ifndef PLAYING_SONG
#define PLAYING_SONG

#include <QObject>
#include <vector>
#include "music_player.h"
#include "hotspots.h"

class Song;

/* This class is used to record Second Counts
 *
 * There should only ever be one instance of it:
 *      Playing_Song::get()
 *
 *  To connect: &Playing_Song::get()
 *
 */

class Playing_Song : public QObject
{
    Q_OBJECT

public:

    static Playing_Song &get()
    {
        static Playing_Song ps;
        return ps;
    }

    // changes the curSong to the song
    // does not play
    void replace_song(Song *song_);

    // throws an Error if cant play the song
    void play();

    void pause();

    // throws error if past total duration
    void change_position(int new_pos);
    void change_pos_relative(int relative_pos);

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

    Phonon::MediaObject *get_media_object()
        {return mp.get_media_object();}

public slots:
    void almostdone_playsong();
    // hotspots
    void set_hs();
    void next_hs();
    void prev_hs();
    void remove_hs();
    std::vector<int> get_hs_list();

signals:
    void became_paused();
    void started_playing();
    //void new_total_duration(int);
    void AlmostDone_PlaySong();
    void song_changed(Song *, int);

private:
    Playing_Song();
    Hotspot_map hs_map;

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
