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

void SelectObjectSpeed();

void SelectSongSpeed();

void SelectSong();

Direction GetJoystickPosition();

void DrawLine(int x1, int y1, int x2, int y2);

void DrawGlitter(int x, int y);

void EraseGlitter(int x, int y);

void EraseLine(int x1, int y1, int x2, int y2);

void EraseObject(int x, int y);

void NextTick(int timems);

extern BaseObject* gameObjects[20];

#endif
