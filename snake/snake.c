/** @file snake.c
 *  @author Gerry Toft, 53712395
 *  @date 12/10/2017
 *  @brief Defines a snake structure and functions for controlling it.
*/

#include "snake.h"

snake_t create_snake(uint8_t x, uint8_t y) {
    snake_t new_snake;
    new_snake.cur_length = 1;
    new_snake.length = 4;
    new_snake.dir = UP;
    new_snake.tail[0].x = x;
    new_snake.tail[0].y = y;
    return new_snake;
}

void snake_eat(snake_t* snake) {
    if (snake->length < MAX_SNAKE_LENGTH) { 
        snake->length++;
    }
}

void snake_move(snake_t* snake) {
    if (snake->cur_length < snake->length) {
        snake->cur_length++;
    }
    
    uint8_t i = 0;
    for (i = snake->cur_length - 1; i > 0; i--) {
        snake->tail[i].x = snake->tail[i - 1].x;
        snake->tail[i].y = snake->tail[i - 1].y;
    }
    
    if (snake->dir == UP) {
        snake->tail[0].y--;
        if (snake->tail[0].y < 0) {
            snake->tail[0].y = TINYGL_HEIGHT - 1;
        }
    } else if (snake->dir == RIGHT) {
        snake->tail[0].x++;
        if (snake->tail[0].x > TINYGL_WIDTH - 1) {
            snake->tail[0].x = 0;
        }
    } else if (snake->dir == DOWN) {
        snake->tail[0].y++;
        if (snake->tail[0].y > TINYGL_HEIGHT - 1) {
            snake->tail[0].y = 0;
        }
    } else if (snake->dir == LEFT) {
        snake->tail[0].x--;
        if (snake->tail[0].x < 0) {
            snake->tail[0].x = TINYGL_WIDTH - 1;
        }
    }
}
