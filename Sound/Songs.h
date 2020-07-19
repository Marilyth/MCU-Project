/*
 * Songs.cpp
 *
 *  Created on: 07.07.2020
 *      Author: student
 */

#ifndef SONG_H_
#define SONG_H_
#include <Sound/PlaySound.h>

struct Beatmap{
    float bpm;
    char* name;
    ENTRY song[500];
};

extern Beatmap SongList[2];

#endif
