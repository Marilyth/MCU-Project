/*
 * HoldObject.h
 *
 *  Created on: 30.06.2020
 *      Author: student
 */

#ifndef HOLDOBJECT_H_
#define HOLDOBJECT_H_

#include <Visuals/GameDisplay.h>
#include <Utils/BaseObject.h>
#include <Utils/Position.h>

//Represents a long object the player needs to deflect by holding the direction for some time
class HoldObject: public BaseObject
{
private:
    //The position of the rear end of the object
    Position* tailPos;
    //How long to hold it for
    int msHold;
    //How long the player has held the Joystick in the correct direction
    int joystickHold;
    //Whether the tail should be moved/reached the visible display
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

        //Move head of the object
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

        //Move tail of the object (black to erase the trail of the head)
        if (isTailActive)
        {
            DrawLine(tailPos->x, tailPos->y, tailPos->x + x_dir * progress,
                     tailPos->y + y_dir * progress, true);
            tailPos->x += x_dir * progress;
            tailPos->y += y_dir * progress;
        }

        //Checks if the player has held the direction for long enough
        if ((double) totalMsPassed / msDuration > 0.85
                && shieldDirection == direction)
        {
            joystickHold += msPassed;

            //Player has held the object for long enough
            if (joystickHold >= msHold - 50)
            {
                DrawLine(tailPos->x, tailPos->y, pos->x, pos->y, true);
                state = hit;
            }

            //Player has failed the object deflection
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
