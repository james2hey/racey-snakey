/** @file food.c
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 12/10/2017
 *  @brief Functions to control the food.
*/

#include "food.h"

/** Generates a random integer between 0 and max-1. The algorithm is
 *  taken from http://wiki.osdev.org/Random_Number_Generator
 *  @param seed the seed from which to generate the random number
 *  @param max an upper bound for the returned value
 *  @return the generated random integer
*/
static uint8_t random(uint16_t seed, uint8_t max)
{
    static uint32_t random_seed = 0;
    random_seed = random_seed + seed * 1103515245 + 12345;
    return (random_seed / 65536) % (max + 1);
}

/** Checks whether a given position is one of the forbidden positions
 *  @param posn the position to check. Given as a single number
 *  calculated from the coordinates (x, y) as TINYGL_WIDTH * y + x
 *  @param num_forbidden the number of forbidden positions
 *  @param forbidden a list of the forbidden positions
 *  @return true if the position is forbidden, false otherwise
*/
static bool check_forbidden(uint8_t posn, uint8_t num_forbidden, tinygl_point_t* forbidden)
{
    uint8_t i = 0;
    for (i = 0; i < num_forbidden; i++) {
        if (posn == TINYGL_WIDTH * forbidden[i].y + forbidden[i].x) {
            return true;
        }
    }
    return false;
}

/** Generates a new position for food, which isn't any of the
 *  given forbidden positions
 *  @param num_forbidden the number of forbidden positions
 *  @param forbidden the positions which food is not allowed to be
 *  @return the coordinates for the new position
*/
tinygl_point_t new_food(uint8_t num_forbidden, tinygl_point_t* forbidden)
{
    uint8_t food_posn = random(timer_get(), TINYGL_WIDTH * TINYGL_HEIGHT - 1);
    
    while (check_forbidden(food_posn, num_forbidden, forbidden)) {
        food_posn = (food_posn + 1) % (TINYGL_WIDTH * TINYGL_HEIGHT);
    }
    
    tinygl_point_t food_point;
    food_point.x = food_posn % TINYGL_WIDTH;
    food_point.y = food_posn / TINYGL_WIDTH;
    
    return food_point;
}
