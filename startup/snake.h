/** @file snake.h
 *  @author Gerry Toft, 53712395
 *  @date 12/10/2017
 *  @brief Defines a snake structure and functions for controlling it.
*/

#include "system.h"
#include "tinygl.h"

#define MAX_SNAKE_LENGTH 8
enum directions {UP, RIGHT, DOWN, LEFT};

typedef struct snake_s {
    uint8_t cur_length;
    uint8_t length;
    uint8_t dir;
    tinygl_point_t tail[MAX_SNAKE_LENGTH];
} snake_t;

snake_t create_snake(uint8_t x, uint8_t y);

void snake_eat(snake_t* snake);

tinygl_point_t new_head_posn(uint8_t dir, snake_t* snake);

void snake_move(snake_t* snake);

void snake_draw(snake_t* snake);
