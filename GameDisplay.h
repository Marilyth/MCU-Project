/*
 * Test.cpp
 *
 *  Created on: 31.03.2020
 *      Author: student
 */
#ifndef GameDisplay
#define GameDisplay

#include <Utils/BaseObject.h>

void StartDisplay();

void DrawObject(int x, int y);

void DrawShield(int radius);

void DrawLine(int x1, int y1, int x2, int y2);

void EraseLine(int x1, int y1, int x2, int y2);

void EraseObject(int x, int y);

void NextTick(int timems);

//Space for up to 100 objects of hit or hold
extern BaseObject* gameObjects[100];

#endif
