#include "playing_song.h"

#include <QDateTime>

#include "error.h"
#include "song.h"
#include "database.h"
#include "error.h"

const int min_milli_secs_c = 1000;

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
    {
        return;
    }

    mp.play_song();

    start_time = QDateTime::currentMSecsSinceEpoch();

    paused = false;

    emit started_playing();
}

void Playing_Song::pause()
{
    mp.pause_song();
    update_position();
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
    if (!paused && song_exists() && cur_time - start_time > min_milli_secs_c)
    {
        long start_pos = position;

        position = mp.curSong->currentTime();
        long end_pos = position;

        int total_time_since_last_save = cur_time-start_time;

        cur_playing->add_secs(end_pos - start_pos);

        Database::get()->save_sec_count(cur_playing->get_id(), start_pos, end_pos);
        emit message(QString("Saving Sec Count: " + cur_playing->get_name() +
                             ". Seconds: " + QString::number(total_time_since_last_save/1000) +
                             ". Position diff: " + QString::number((end_pos - start_pos)/1000)));

        start_time = cur_time;
    }
}

qint64 Playing_Song::get_position()
{
    return  mp.curSong->currentTime();
}

// changes the curSong to the song
// does not play
void Playing_Song::replace_song(Song *song_)
{
    if (song_exists())
    {
        update_position();
        mp.remove_song();
        cur_playing->stop_playing();
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
    song_->start_playing();
}

// throws error if past total duration
void Playing_Song::change_position(int new_pos_millisecs)
{
    if (new_pos_millisecs > totalDuration || new_pos_millisecs < 0)
        throw new Error("Cant Seek to that position:" + QString::number(new_pos_millisecs));

    update_position();

    mp.move_to(new_pos_millisecs);

    position = new_pos_millisecs;
}

// This Has a Glitch!!!
void Playing_Song::change_pos_relative(int relative_seconds)
{
    try
    {
        // watch this!!! We might want to insert qint64s and change those..
        change_position((int)(get_position()*1000) + relative_seconds);
        // gotta check this!!!!
    }
    catch (Error &e)
    {
        qDebug() << e.msg;
    }

}

void Playing_Song::set_hs()
{
    hs_map.add_hotspot(cur_playing->get_id(), get_position());
    Database::get()->add_hotspot(cur_playing->get_id(), get_position());
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
       // e.print_error_msg();
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
        //e.print_error_msg();
        change_position(0);
    }
}

void Playing_Song::remove_next_hs()
{
    try
    {
        int oldHotspot;
        Hotspots &hotspots = hs_map.get_hotspots(cur_playing->get_id());
        oldHotspot = hotspots.get_next((get_position()));
        hotspots.remove(oldHotspot);
        Database::get()->delete_hotspot(cur_playing->get_id(), oldHotspot);

        emit hs_added();
    }
    catch (Error &e)
    {
        e.print_error_msg();
    }
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
