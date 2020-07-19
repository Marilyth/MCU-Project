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

class GameLogic: public task
{
public:
    // The base class 'task' has to be called with
    // the name of the task, and optionally (as the second
    // parameter) the stack size of the task.
    GameLogic(const char * name) :
            task(name)
    {
    }

    // This is the task code, which is
    // run by the multitasking kernel
    void run() override
    {
        while (1)
        {
            NextTick(monitor.millis());
        }
    }
};


#endif /* TASKS_GAMELOGIC_H_ */
