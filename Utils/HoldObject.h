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
#include <Utils/SuccessObject.h>

class HoldObject: public BaseObject
{
private:
    Position* tailPos;
    int msHold;
    int joystickHold;
    bool isTailActive;

public:
    HoldObject(Direction d, int millis, int millisHold) :
            BaseObject(d, millis)
    {
        tailPos = new Position(this->pos->x, this->pos->y);
        msHold = millisHold;
        joystickHold = 0;
        isTailActive = false;

        //Slide this Object into the next available slot
        for (int i = 0; i < 20; i++)
        {
            if (gameObjects[i] == nullptr)
            {
                gameObjects[i] = this;
                break;
            }
        }
    }

    void move(int msPassed, Direction shieldDirection)
    {
        double progress = (double) msPassed / msDuration;

        if (!isTailActive)
            isTailActive = (((double) totalMsPassed / msDuration) + progress
                    - ((double) msHold / msDuration)) > 0;

        totalMsPassed += msPassed;

        if (totalMsPassed < msDuration * 1.15)
        {
            if ((double) totalMsPassed / msDuration <= 0.85
                    || shieldDirection != direction)
            {
                DrawLine(pos->x, pos->y, pos->x + x_dir * progress,
                         pos->y + y_dir * progress);
                pos->x += x_dir * progress;
                pos->y += y_dir * progress;
            }
        }

        if (isTailActive)
        {
            DrawLine(tailPos->x, tailPos->y, tailPos->x + x_dir * progress,
                     tailPos->y + y_dir * progress, true);
            tailPos->x += x_dir * progress;
            tailPos->y += y_dir * progress;
        }

        if ((double) totalMsPassed / msDuration > 0.85
                && shieldDirection == direction)
        {
            joystickHold += msPassed;
            if (joystickHold >= msHold - 50)
            {
                DrawLine(tailPos->x, tailPos->y, pos->x, pos->y, true);
                state = hit;
            }
            else if (joystickHold + (msDuration + msHold) * 1.15 - totalMsPassed
                    < msHold - 50)
            {
                DrawLine(tailPos->x, tailPos->y, pos->x, pos->y, true);
                state = missed;
            }
        }
    }

    bool isDone()
    {
        return totalMsPassed > (msDuration + msHold) * 1.15;
    }

    ~HoldObject(){
        delete tailPos;
    }
};

#endif /* HOLDOBJECT_H_ */
