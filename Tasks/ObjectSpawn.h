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

//Creates objects which correspond to the beat of the song
class ObjectSpawn: public task
{
private:
    Direction lastDirection;
    bool wasHold = true;
public:
    ObjectSpawn(const char * name) :
            task(name)
    {
    }

    //Creates an object in a random direction different form the last one
    void onBeatTriggered(int duration)
    {
        BaseObject* obj;
        Direction direction;

        //Whether the beat was long enough to warrant a hold object
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

    //Iterates through the song beats and creates objects if they are to be played and not a pause
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
