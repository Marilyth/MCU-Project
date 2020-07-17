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
    int bpm;
    int scale;
    char* name;
    ENTRY song[200];
};

extern Beatmap SuperMario;

extern Beatmap SongList[1];

#endif
