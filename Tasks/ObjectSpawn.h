/*
 * ObjectSpawn.h
 *
 *  Created on: 19.07.2020
 *      Author: student
 */

#ifndef TASKS_OBJECTSPAWN_H_
#define TASKS_OBJECTSPAWN_H_

#include <Utils/BaseObject.h>
#include <Utils/HitObject.h>
#include <Utils/HoldObject.h>
#include <Sound/PlaySound.h>
#include "task.h"
#include <Sound/Songs.h>
#include <stdlib.h>
#include <Utils/StaticBase.h>

class ObjectSpawn: public task
{
private:
    Direction lastDirection;
    bool wasHold = true;
public:
    // The base class 'task' has to be called with
    // the name of the task, and optionally (as the second
    // parameter) the stack size of the task.
    ObjectSpawn(const char * name) :
            task(name)
    {
    }

    void onBeatTriggered(int duration)
    {
        BaseObject* obj;
        Direction direction;
        bool isHold = duration >= 250 / songSpeedMultiplicator;

        do
        {
            direction = static_cast<Direction>(rand() % 4);
        }
        while (direction == lastDirection); //&& !(!isHold && !wasHold));

        if (isHold)
            obj = new HoldObject(direction, objectSpeedMs, duration);
        else
            obj = new HitObject(direction, objectSpeedMs);

        lastDirection = direction;
        wasHold = isHold;
    }

// This is the task code, which is
// run by the multitasking kernel
    void run() override
    {
        srand(seed);
        int idx = 0;
        int startTime = monitor.millis();

        while (SongList[songSelection].song[idx].beatDivider != 0)
        {
            while ((monitor.millis() - startTime)
                    <= SongList[songSelection].song[idx].dueAtMS)
                ;
            int overdue = (monitor.millis() - startTime)
                    - SongList[songSelection].song[idx].dueAtMS;
            if (SongList[songSelection].song[idx].t != P && SongList[songSelection].song[idx].isPlayed){
                onBeatTriggered(
                        SongList[songSelection].song[idx].durationMS - overdue);
            }
            idx++;
        }
    }
};


#endif /* TASKS_OBJECTSPAWN_H_ */
