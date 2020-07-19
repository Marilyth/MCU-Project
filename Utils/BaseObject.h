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

//Represents (in most cases) a movable object which needs to be deflected
class BaseObject
{
protected:
    //Current position of the object
    Position* pos;
    //Direction the object moves towards
    double x_dir;
    double y_dir;
    //How long the object needs from border to the player shield
    int msDuration;
    //How much time has passed since the creation of the object
    int totalMsPassed;

public:
    //Whether the object was hit, missed, yet to be determined or should be ignore (SuccessObject)
    State state;

    //The direction the object comes from
    Direction direction;

    BaseObject(Direction d, int millis)
    {
        state = undetermined;
        direction = d;
        int x = 0;
        int y = 0;

        //Sets up coordinates to represent the direction etc.
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

    //Represents a tick for the object.
    //By knowing how much time has passed, the object knows how far to move
    //By knowing the direction of the shield, the object knows whether it was deflected
    virtual void move(int msPassed, Direction shieldDirection) = 0;

    //Represents if the object reached the center/time is up
    virtual bool isDone() = 0;

    virtual ~BaseObject(){
        delete pos;
    }
};

#endif /* BASEOBJECT_H_ */
