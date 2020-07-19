/*
 * StaticBase.h
 *
 *  Created on: 17.07.2020
 *      Author: student
 */

#ifndef UTILS_STATICBASE_H_
#define UTILS_STATICBASE_H_

#include "task_monitor.h"

//Meta Information that may be used statically in multiple locations
extern int objectSpeedMs;
extern float songSpeedMultiplicator;
extern int seed;
extern int songSelection;
extern int songCount;

extern task_monitor monitor;

#endif /* UTILS_STATICBASE_H_ */
