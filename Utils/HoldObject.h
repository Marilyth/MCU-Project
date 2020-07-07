/*
 * HoldObject.h
 *
 *  Created on: 30.06.2020
 *      Author: student
 */

#ifndef HOLDOBJECT_H_
#define HOLDOBJECT_H_

#include <GameDisplay.h>
#include <Utils/BaseObject.h>
#include <Utils/Position.h>

class HoldObject: public BaseObject
{
private:
    Position* tailPos;
    int msHold;
    bool isTailActive;

public:
    HoldObject(Direction d, int millis, int millisHold) :
            BaseObject(d, millis)
    {
        tailPos = new Position(this->pos->x, this->pos->y);
        msHold = millisHold;
        isTailActive = false;

        //Slide this Object into the next available slot
        for (int i = 0; i < 100; i++)
        {
            if (gameObjects[i] == nullptr)
            {
                gameObjects[i] = this;
                break;
            }
        }
    }

    void move(int msPassed)
    {
        double progress = (double) msPassed / msDuration;

        if (!isTailActive)
            isTailActive = (((double) totalMsPassed / msDuration) + progress
                    - ((double) msHold / msDuration)) > 0;

        totalMsPassed += msPassed;

        if (totalMsPassed < msDuration)
        {
            DrawLine(pos->x, pos->y, pos->x + x_dir * progress,
                     pos->y + y_dir * progress);
            pos->x += x_dir * progress;
            pos->y += y_dir * progress;
        }

        if (isTailActive > 0)
        {
            EraseLine(tailPos->x, tailPos->y, tailPos->x + x_dir * progress,
                      tailPos->y + y_dir * progress);
            tailPos->x += x_dir * progress;
            tailPos->y += y_dir * progress;
        }
    }

    bool isDone()
    {
        return totalMsPassed > msDuration + msHold;
    }
};

#endif /* HOLDOBJECT_H_ */
