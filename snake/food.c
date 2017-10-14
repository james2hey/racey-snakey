/** @file food.c
 *  @author Gerry Toft, 53712395
 *  @date 12/10/2017
 *  @brief Functions to control the food.
*/

#include "food.h"

/* from http://wiki.osdev.org/Random_Number_Generator
*/
static uint8_t random(uint16_t seed, uint8_t max) {
    static uint32_t random_seed = 0;
    random_seed = random_seed + seed * 1103515245 + 12345;
    return (random_seed / 65536) % (max + 1);
}

static bool check_forbidden(uint8_t posn, uint8_t num_forbidden, tinygl_point_t* forbidden) {
    uint8_t i = 0;
    for (i = 0; i < num_forbidden; i++) {
        if (posn == TINYGL_WIDTH * forbidden[i].y + forbidden[i].x) {
            return true;
        }
    }
    return false;
}

//NOTE TO FUTURE SELF: this algorithm is a bit wonky and should be improved
tinygl_point_t new_food(uint8_t num_forbidden, tinygl_point_t* forbidden) {
    //uint8_t food_posn = random(timer_get(), TINYGL_WIDTH * TINYGL_HEIGHT - 1);
    
    while (check_forbidden(food_posn, num_forbidden, forbidden)) {
        food_posn = (food_posn + 1) % (TINYGL_WIDTH * TINYGL_HEIGHT);
    }
    
    tinygl_point_t food_point;
    food_point.x = food_posn % TINYGL_WIDTH;
    food_point.y = food_posn / TINYGL_WIDTH;
    
    return food_point;
}
