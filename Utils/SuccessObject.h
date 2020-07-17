/*
 * SuccessObject.h
 *
 *  Created on: 17.07.2020
 *      Author: student
 */

#ifndef UTILS_SUCCESSOBJECT_H_
#define UTILS_SUCCESSOBJECT_H_

#include <GameDisplay.h>
#include <Utils/BaseObject.h>
#include <Utils/Position.h>

class SuccessObject: public BaseObject
{
private:
    bool wasDrawn;
public:
    SuccessObject(Direction d, int millis) :
            BaseObject(d, millis)
    {
        wasDrawn = false;

        //Slide this Object into the next available slot
        for (int i = 0; i < 20; i++)
        {
            if (gameObjects[i] == nullptr)
            {
                gameObjects[i] = this;
                break;
            }

            if (i == 20)
                int test = 0;
        }
    }

    void move(int msPassed, Direction shieldDirection)
    {
        totalMsPassed += msPassed;

        if (!wasDrawn)
        {
            switch (direction)
            {
            case up:
                DrawGlitter(61, 48);
                break;
            case down:
                DrawGlitter(61, 78);
                break;
            case left:
                DrawGlitter(48, 61);
                break;
            case right:
                DrawGlitter(78, 61);
                break;
            }
        }

        if (isDone())
        {
            state = ignore;

            switch (direction)
            {
            case up:
                EraseGlitter(61, 48);
                break;
            case down:
                EraseGlitter(61, 78);
                break;
            case left:
                EraseGlitter(48, 61);
                break;
            case right:
                EraseGlitter(78, 61);
                break;
            }
        }
    }

    bool isDone()
    {
        return totalMsPassed > msDuration;
    }
};

#endif /* UTILS_SUCCESSOBJECT_H_ */