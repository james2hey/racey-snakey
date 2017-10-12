/** @file snake.c
 *  @author Gerry Toft, 53712395
 *  @date 12/10/2017
 *  @brief Defines a snake structure and functions for controlling it.
*/

#include "snake.h"

snake_t create_snake(uint8_t x, uint8_t y) {
    snake_t new_snake;
    new_snake.cur_length = 1;
    new_snake.length = 1;
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

tinygl_point_t new_head_posn(uint8_t dir, snake_t* snake) {
    tinygl_point_t head_posn = snake->tail[0];
    if (dir == UP) {
        head_posn.y--;
        if (head_posn.y < 0) {
            head_posn.y = TINYGL_HEIGHT - 1;
        }
    } else if (dir == RIGHT) {
        head_posn.x++;
        if (head_posn.x > TINYGL_WIDTH - 1) {
            head_posn.x = 0;
        }
    } else if (dir == DOWN) {
        head_posn.y++;
        if (head_posn.y > TINYGL_HEIGHT - 1) {
            head_posn.y = 0;
        }
    } else if (dir == LEFT) {
        head_posn.x--;
        if (head_posn.x < 0) {
            head_posn.x = TINYGL_WIDTH - 1;
        }
    }
    
    return head_posn;
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
    
    snake->tail[0] = new_head_posn(snake->dir, snake);
}
