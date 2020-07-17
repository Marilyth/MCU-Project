/*****************************************************************************
 *
 * Copyright (C) 2013 - 2017 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 * * Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 *
 * MSP432 empty main.c template
 *
 ******************************************************************************/

#ifndef MAIN
#define MAIN

#include <GameDisplay.h>
#include <Utils/HoldObject.h>
#include <Utils/HitObject.h>
#include <Utils/BaseObject.h>
#include <Sound/PlaySound.h>
#include <Utils/StaticBase.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <Sound/Songs.h>
#include "math.h"
#include "task.h"
#include "gpio_msp432.h"
#include "task_monitor.h"
using namespace std;

task_monitor monitor;
int objectSpeedMs = 1000;
double songSpeedMultiplicator = 1;
int songSelection = 0;
int songCount = 1;
int objectTimer = 0;

class SongTask: public task
{
private:
    PlaySound p;
public:
    // The base class 'task' has to be called with
    // the name of the task, and optionally (as the second
    // parameter) the stack size of the task.
    SongTask(const char * name) :
            task(name)
    {
    }

    // This is the task code, which is
    // run by the multitasking kernel
    void run() override
    {
        int idx = 0;

        //Sync Object Spawner with melody
        sleep(objectSpeedMs);
        int startTime = monitor.millis();

        while (SongList[songSelection].song[idx].beatDivider != 0)
        {
            auto currentEntry = SongList[songSelection].song[idx];

            while ((monitor.millis() - startTime)
                    <= SongList[songSelection].song[idx].dueAtMS)
                ;
            int overdue = (monitor.millis() - startTime)
                    - SongList[songSelection].song[idx].dueAtMS;
            if (SongList[songSelection].song[idx].t != P)
                p.playTone(
                        SongList[songSelection].song[idx].t,
                        SongList[songSelection].song[idx].durationMS - overdue);
            idx++;
        }

        //p.playMelody(SongList[songSelection].song, SongList[songSelection].bpm, SongList[songSelection].scale);
    }
};

class GameTask: public task
{
public:
    // The base class 'task' has to be called with
    // the name of the task, and optionally (as the second
    // parameter) the stack size of the task.
    GameTask(const char * name) :
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

class ObjectSpawn: public task
{
private:
    Direction lastDirection;
    bool wasHold = true;
public:
    // The base class 'task' has to be called with
    // the name of the task, and optionally (as the second
    // parameter) the stack size of the task.
    ObjectSpawn(const char * name) :
            task(name)
    {
    }

    void onBeatTriggered(int duration)
    {
        BaseObject* obj;
        Direction direction;
        bool isHold = duration > 300 * songSpeedMultiplicator;

        do
        {
            direction = static_cast<Direction>(rand() % 4);
        }
        while (direction == lastDirection); //&& !(!isHold && !wasHold));

        if (isHold)
            obj = new HoldObject(direction, objectSpeedMs, duration);
        else
            obj = new HitObject(direction, objectSpeedMs);

        lastDirection = direction;
        wasHold = isHold;
    }

// This is the task code, which is
// run by the multitasking kernel
    void run() override
    {
        int idx = 0;
        int startTime = monitor.millis();
        objectTimer = startTime;

        while (SongList[songSelection].song[idx].beatDivider != 0)
        {
            auto currentEntry = SongList[songSelection].song[idx];
            while ((monitor.millis() - startTime)
                    <= SongList[songSelection].song[idx].dueAtMS)
                ;
            int overdue = (monitor.millis() - startTime)
                    - SongList[songSelection].song[idx].dueAtMS;
            if (SongList[songSelection].song[idx].t != P)
                onBeatTriggered(
                        SongList[songSelection].song[idx].durationMS - overdue);
            idx++;
        }
    }
};

int main()
{
    srand(time(0));
    StartDisplay();
    ObjectSpawn o(
            "Creates random objects the player needs to deflect with their joystick");
    o.start();
    GameTask g("Handles game ticks, drawing and logic");
    g.start();
    SongTask s("Plays the song and creates objects based on the beats");
    s.start();

    task_monitor monitor;
    monitor.start();
    task::start_scheduler();

    return 0;
}

#endif
