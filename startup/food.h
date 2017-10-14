/** @file food.h
 *  @author Gerry Toft, 53712395
 *  @date 12/10/2017
 *  @brief Functions to control the food.
*/

#include "system.h"
#include "tinygl.h"
#include "timer.h"

tinygl_point_t new_food(uint8_t num_forbidden, tinygl_point_t* forbidden);
