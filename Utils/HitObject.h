/*
 * HitObject.h
 *
 *  Created on: 30.06.2020
 *      Author: student
 */

#ifndef HITOBJECT_H_
#define HITOBJECT_H_

#include <GameDisplay.h>
#include <Utils/Position.h>
#include <Utils/BaseObject.h>

class HitObject: public BaseObject
{
public:
    HitObject(Direction d, int millis) :
            BaseObject(d, millis)
    {
        //Slide this Object into the next available slot
        for (int i = 0; i < 20; i++)
        {
            if (gameObjects[i] == nullptr)
            {
                gameObjects[i] = this;
                break;
            }

            if(i == 20)
                int test = 0;
        }
    }

    void move(int msPassed, Direction shieldDirection)
    {
        EraseObject(pos->x, pos->y);

        double progress = (double) msPassed / msDuration;
        totalMsPassed += msPassed;

        if (totalMsPassed < msDuration*1.15)
        {
            //Player deflected this object in time
            if ((double) totalMsPassed / msDuration > 0.85)
            {
                if (shieldDirection == direction)
                {
                    state = hit;
                    return;
                }
            }

            pos->x += x_dir * progress;
            pos->y += y_dir * progress;
            DrawObject(pos->x, pos->y);
        }

        //Player failed to deflect the object in time
        else
        {
            state = missed;
        }
    }

    bool isDone()
    {
        return totalMsPassed > msDuration;
    }
};

#endif /* HITOBJECT_H_ */
