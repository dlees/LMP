#include "playing_song.h"

#include <QDateTime>

#include "error.h"
#include "song.h"
#include "database.h"

const int min_secs_c = 0;

Playing_Song::Playing_Song()
    : cur_playing(0),
      paused(true),
      totalDuration(0),
      position(0)
{

}

// throws an Error if cant play the song
void Playing_Song::play()
{
    if (mp.cant_play())
        throw new Error("Can't play " + cur_playing->get_name());

    mp.play_song();

    start_time = QDateTime::currentMSecsSinceEpoch();

    paused = false;

    emit started_playing();
}

void Playing_Song::pause()
{
    update_position();
    mp.pause_song();
    paused = true;

    emit became_paused();
}

// prints second count to the file
void Playing_Song::update_position()
{
    qint64 cur_time = QDateTime::currentMSecsSinceEpoch();

    // don't update if it's paused, there is no song,
    // or it hasn't been a second yet
    // this logic should be changed to the oposite way! 381!
    if (!paused && song_exists() && cur_time - start_time > min_secs_c)
    {
        long start_sec = position;
        position += cur_time - start_time;
        long end_sec = position;

        start_time = cur_time;

        cur_playing->add_secs(end_sec - start_sec);

        Database::get()->save_sec_count(cur_playing->get_id(), start_sec, end_sec);
    }
}

qint64 Playing_Song::get_position()
{
    qint64 cur_time = QDateTime::currentMSecsSinceEpoch();

    return position + cur_time - start_time;
}

// changes the curSong to the song
// does not play
void Playing_Song::replace_song(Song *song_)
{
    if (song_exists())
    {
        update_position();
        mp.remove_song();
    }

    set_song(song_);
}

bool Playing_Song::song_exists()
{
    return cur_playing != 0;
}

void Playing_Song::set_song(Song* song_)
{
    cur_playing = song_;

    mp.set_song(cur_playing->get_filename());

    totalDuration = mp.get_duration();

    position = 0;
    paused = true;
}

// throws error if past total duration
void Playing_Song::change_position(int new_pos)
{
    if (new_pos > totalDuration)
        throw new Error("Cant Seek to that position");

    update_position();

    mp.move_to(new_pos);

    position = new_pos;
}

void Playing_Song::change_pos_relative(int relative_pos)
{
    // watch this!!! We might want to insert qint64s and change those..
    change_position((int)(get_position()*1000) + relative_pos);
    // gotta check this!!!!
}

void Playing_Song::set_hs()
{

}

void Playing_Song::next_hs(){}

void Playing_Song::prev_hs(){}

void Playing_Song::remove_hs(){}

bool Playing_Song::is_paused()
{
    return paused;
}

bool Playing_Song::is_finished()
{
    return totalDuration < get_position();
}

void Playing_Song::vol_up()
{
    set_volume_percent(get_volume_percent() + 5);
}

void Playing_Song::vol_down()
{
    set_volume_percent(get_volume_percent() - 5);
}

void Playing_Song::set_volume_percent(int percent)
{
    if (percent > 100)
        percent = 100;

    else if (percent < 0)
        percent = 0;

    mp.change_volume(percent);
}

int Playing_Song::get_volume_percent()
{
    return mp.get_volume_percent();
}

void Playing_Song::close(){}
