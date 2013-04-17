#include "playing_song.h"

#include <QDateTime>

#include "error.h"
#include "song.h"
#include "database.h"
#include "error.h"

const int min_secs_c = 0;

Playing_Song::Playing_Song()
    : cur_playing(0),
      paused(true),
      totalDuration(0),
      position(0)
{
    connect(&mp, SIGNAL(almost_done()),
            this, SLOT(almostdone_playsong()));

    connect(get_media_object(), SIGNAL(totalTimeChanged(qint64)),
            this, SLOT(duration_changed(qint64)));
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
    if (get_media_object()->state() != Phonon::PlayingState)
    {
        return position;
    }
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

    position = 0;
    paused = true;

    emit song_changed(song_);
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
    hs_map.add_hotspot(cur_playing->get_id(), get_position());
    emit hs_added();
}

void Playing_Song::next_hs()
{
    qDebug() << "Next HS";
    try
    {
        change_position(hs_map.get_hotspots(cur_playing->get_id()).get_next(get_position()+1000));
    }
    catch (Error &e)
    {
        e.print_error_msg();
    }
}

void Playing_Song::prev_hs()
{
    qDebug() << "Prev HS";
    try
    {
        change_position(hs_map.get_hotspots(cur_playing->get_id()).get_prev(get_position()-1000));
    }
    catch (Error &e)
    {
        e.print_error_msg();
    }
}

void Playing_Song::remove_hs()
{
    Hotspots &hotspots = hs_map.get_hotspots(cur_playing->get_id());
    hotspots.remove(hotspots.get_next(get_position()));
    emit hs_added();
}

std::vector<int> Playing_Song::get_hs_list()
{
    std::vector<int> hotspots;
    try
    {
         hotspots = hs_map.get_hotspots(cur_playing->get_id()).get_hotspots();
         return hotspots;
    }
    catch (Error &e)
    {
        qDebug() << e.msg;
        // default is the beginning time
        //hotspots.push_back(0);
        return hotspots;
    }
}

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


void Playing_Song::almostdone_playsong()
{
    emit AlmostDone_PlaySong();
    qDebug() << "Playing_Song: Almost Done" ;
}

void Playing_Song::close(){}

void Playing_Song::duration_changed(qint64 new_dur)
{
    totalDuration = new_dur;
    emit new_total_duration(totalDuration);
}
