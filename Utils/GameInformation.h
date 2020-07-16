/*
 * GameInformation.h
 *
 *  Created on: 16.07.2020
 *      Author: student
 */

#ifndef UTILS_GAMEINFORMATION_H_
#define UTILS_GAMEINFORMATION_H_
#include <Utils/BaseObject.h>

class GameInformation{
public:
    int score;
    int combo;
    int hits;
    int misses;
    double accuracy;
    Direction shieldDirection;

    GameInformation(){
        score = 0;
        combo = 0;
        hits = 0;
        misses = 0;
        accuracy = 0;
        shieldDirection = idle;
    }
};


#endif /* UTILS_GAMEINFORMATION_H_ */
