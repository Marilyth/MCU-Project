/*
 * BaseObject.h
 *
 *  Created on: 30.06.2020
 *      Author: student
 */

#ifndef BASEOBJECT_H_
#define BASEOBJECT_H_

#include <Visuals/GameDisplay.h>
#include <Utils/Position.h>

enum Direction
{
    up, down, left, right, idle
};

enum State
{
    undetermined, hit, missed, ignore
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
    State state;
    Direction direction;

    BaseObject(Direction d, int millis)
    {
        state = undetermined;
        direction = d;
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
        if (x_dir != 0)
            x_dir += x_dir > 0 ? -10 : 10;
        y_dir = (63 - pos->y);
        if (y_dir != 0)
            y_dir += y_dir > 0 ? -10 : 10;
    }

    virtual void move(int msPassed, Direction shieldDirection) = 0;

    virtual bool isDone() = 0;

    virtual ~BaseObject(){
        delete pos;
    }
};

#endif /* BASEOBJECT_H_ */
