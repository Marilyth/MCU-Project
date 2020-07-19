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

class PlaySong: public task
{
private:
    PlaySound p;
public:
    // The base class 'task' has to be called with
    // the name of the task, and optionally (as the second
    // parameter) the stack size of the task.
    PlaySong(const char * name) :
            task(name)
    {
    }

    // This is the task code, which is
    // run by the multitasking kernel
    void run() override
    {
        int idx = 0;

        //Sync Object Spawner with melody
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
