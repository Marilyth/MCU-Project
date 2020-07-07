/*
 * BaseObject.h
 *
 *  Created on: 30.06.2020
 *      Author: student
 */

#ifndef BASEOBJECT_H_
#define BASEOBJECT_H_

#include <GameDisplay.h>
#include <Utils/Position.h>

enum Direction
{
    up, down, left, right
};

class BaseObject
{
protected:
    Position* pos;
    double x_dir;
    double y_dir;
    int msDuration;
    int totalMsPassed;

public:
    BaseObject(Direction d, int millis)
    {
        int x = 0;
        int y = 0;
        if (d == up)
        {
            x = 63;
        }
        else if (d == down)
        {
            x = 63;
            y = 127;
        }
        else if (d == left)
        {
            y = 63;
        }
        else if (d == right)
        {
            x = 127;
            y = 63;
        }
        pos = new Position(x, y);
        msDuration = millis;
        totalMsPassed = 0;
        x_dir = (63 - pos->x);
        y_dir = (63 - pos->y);
    }

    virtual void move(int msPassed) = 0;

    virtual bool isDone() = 0;
};

#endif /* BASEOBJECT_H_ */
