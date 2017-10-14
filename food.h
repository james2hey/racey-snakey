/** @file food.h
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 12/10/2017
 *  @brief Functions to control the food.
*/

#ifndef FOOD_H
#define FOOD_H

#include "system.h"
#include "tinygl.h"
#include "timer.h"

/** Generates a new position for food, which isn't any of the
 *  given forbidden positions
 *  @param num_forbidden the number of forbidden positions
 *  @param forbidden the positions which food is not allowed to be
 *  @return the coordinates for the new position
*/
tinygl_point_t new_food(uint8_t num_forbidden, tinygl_point_t* forbidden);

#endif
