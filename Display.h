/*
 * Test.cpp
 *
 *  Created on: 31.03.2020
 *      Author: student
 */
#ifndef Display
#define Display

void StartDisplay();

void DrawObject(int x, int y);

void DrawShield(int radius);

void EraseObject(int x, int y);

void NextTick(int timems);

struct Position
{
    double x;
    double y;
    Position(double _x, double _y) :
            x(_x), y(_y)
    {
    }
};

struct Bullet
{
    Position* pos;
    double x_dir;
    double y_dir;
    int milliseconds;
    int totalMsPassed;

    Bullet(double x, double y, int millis)
    {
        pos = new Position(x, y);
        milliseconds = millis;
        totalMsPassed = 0;
        x_dir = (64 - pos->x);
        y_dir = (64 - pos->y);
    }

    void move(int msPassed)
    {
        EraseObject(pos->x, pos->y);

        double progress = (double) msPassed / milliseconds;
        totalMsPassed += msPassed;

        if (totalMsPassed < milliseconds)
        {
            pos->x += x_dir * progress;
            pos->y += y_dir * progress;
            DrawObject(pos->x, pos->y);
        }
    }
};

//Space for up to 50 bullets
extern Bullet* bullets[50];

#endif
