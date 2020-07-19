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

void SelectObjectSpeed();

void SelectSongSpeed();

void SelectSeed();

void SelectSong();

bool IsJoystickPressed();

Direction GetJoystickPosition();

void DrawLine(int x1, int y1, int x2, int y2, bool erase = false);

void DrawGlitter(int x, int y, bool erase = false);

void DrawArrow(int x, int y, Direction d, bool erase = false);

void DrawOptions();

void DrawSongInfo();

void DrawHeart();

void NextTick(int timems);

extern BaseObject* gameObjects[20];

#endif
