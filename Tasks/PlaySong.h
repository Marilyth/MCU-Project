/*
 * PlaySong.h
 *
 *  Created on: 19.07.2020
 *      Author: student
 */

#ifndef TASKS_PLAYSONG_H_
#define TASKS_PLAYSONG_H_

#include <Sound/PlaySound.h>
#include "task.h"
#include <Sound/Songs.h>
#include <Utils/StaticBase.h>

//Plays the beats of the song
class PlaySong: public task
{
private:
    PlaySound p;
public:
    PlaySong(const char * name) :
            task(name)
    {
    }

    //Iterates through the song and plays the tones when they are due
    void run() override
    {
        int idx = 0;

        //Sync melody to start when the first object hits the shield
        sleep(objectSpeedMs);
        int startTime = monitor.millis();

        while (SongList[songSelection].song[idx].beatDivider != 0)
        {
            while ((monitor.millis() - startTime)
                    <= SongList[songSelection].song[idx].dueAtMS)
                ;
            int overdue = (monitor.millis() - startTime)
                    - SongList[songSelection].song[idx].dueAtMS;
            if (SongList[songSelection].song[idx].t != P)
                p.playTone(
                        SongList[songSelection].song[idx].t,
                        SongList[songSelection].song[idx].durationMS - overdue);
            idx++;
        }
    }
};


#endif /* TASKS_PLAYSONG_H_ */
