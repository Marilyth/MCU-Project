/*
 * Songs.cpp
 *
 *  Created on: 07.07.2020
 *      Author: student
 */

#ifndef SONG_H_
#define SONG_H_
#include <main.cpp>
#include <Sound/PlaySound.h>

extern int SuperMarioBPM = 80;
extern ENTRY SuperMario[] = { { e2, 8, true}, { e2, 8 }, { P, 8 }, { e2, 8, true},
                              { P, 8 }, { c2, 8 }, { e2, 4 }, { g2, 4, true},
                              { P, 4 }, { g1, 4, true}, { P, 4 }, { c2, 4, true},
                              { P, 8 }, { g1, 8, true}, { P, 4 }, { e1, 4 },
                              { P, 8 }, { a1, 8 }, { P, 8 }, { h1, 8 },
                              { P, 8 }, { b1, 8 }, { a1, 4, true}, {P, 0}

};

#endif
