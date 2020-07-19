/*
 * GameLogic.h
 *
 *  Created on: 19.07.2020
 *      Author: student
 */

#ifndef TASKS_GAMELOGIC_H_
#define TASKS_GAMELOGIC_H_

#include "task.h"
#include <Utils/StaticBase.h>

//Tells the GameDisplay to handle the next tick and how much time has passed
class GameLogic: public task
{
public:
    GameLogic(const char * name) :
            task(name)
    {
    }

    void run() override
    {
        while (1)
        {
            NextTick(monitor.millis());
        }
    }
};


#endif /* TASKS_GAMELOGIC_H_ */
