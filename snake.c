/** @file snake.c
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 12/10/2017
 *  @brief Defines a snake structure and functions for controlling it.
*/

#include "snake.h"

/** Creates a new snake structure from given initial properties
 *  @param x the x coordinate the snake starts at
 *  @param y the y coordinate the snake starts at
 *  @param length the initial length of the snake
 *  @param dir the snakes initial direction of motion
 *  @return the snake structure
*/
snake_t create_snake(uint8_t x, uint8_t y, uint8_t length, uint8_t dir, uint8_t colour)
{
    snake_t new_snake;
    new_snake.cur_length = 1;
    new_snake.length = length;
    new_snake.dir = dir;
    new_snake.tail[0].x = x;
    new_snake.tail[0].y = y;
    new_snake.colour = colour;
    new_snake.alive = true;
    return new_snake;
}

/** Function to be called when a snake eats food. This increases\
 *  the snake's length up to the maximum length
 *  @param snake the snake which has eaten food
*/
void snake_eat(snake_t* snake)
{
    if (snake->length < MAX_SNAKE_LENGTH) { 
        snake->length++;
    }
}

/** Calculates the position the snake's head will be at after
 *  moving in the given direction
 *  @param dir the direction the snake will move in
 *  @param snake the snake that is moving
 *  @return the new position of the head
*/
tinygl_point_t new_head_posn(uint8_t dir, snake_t* snake)
{
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

/** Updates the position of the snake and its tail. The snake will wrap
 *  through walls.
 *  @param snake the snake to move
*/
void snake_move(snake_t* snake)
{
    if (snake->alive) { 
        if (snake->cur_length < snake->length) {
            snake->cur_length++;
        }
    
        uint8_t i = 0;
        for (i = snake->cur_length - 1; i > 0; i--) {
            snake->tail[i].x = snake->tail[i - 1].x;
            snake->tail[i].y = snake->tail[i - 1].y;
        }
    
        snake->tail[0] = new_head_posn(snake->dir, snake);
    } else if (snake->cur_length > 0) {
        snake->cur_length--;
    }
}

/** Draws the given snake onto the LED matrix
 *  @param snake the snake to draw
*/
void snake_draw(snake_t* snake)
{
    static uint8_t colour_tick = 0;
    
    if (colour_tick % snake->colour < 1) {  
        uint8_t i = 0;
        for (i = 0; i < snake->cur_length; i++) {
            tinygl_draw_point(snake->tail[i], 1);
        }
    }
    
    colour_tick = (colour_tick + 1) % SNAKE_COLOUR_STEPS;
}

/** Detects if the snake's head has collided with the given point
 *  @param snake the snake
 *  @param point the point to check for collision
 *  @return returns true if there is a collision, false otherwise
*/
bool collision(snake_t* snake, tinygl_point_t point)
{
    return (snake->tail[0].x == point.x && snake->tail[0].y == point.y);
}

/** Detects if a snake has collided with the tail of another. If
 *  snake1 = snake2 the function detects if snake1 has collided
 *  with itself.
 *  @param snake1 the snake who's head is checked for collision
 *  @param snake2 the snake who's tail is checked for collision
 *  @return returns true if the snakes have collided, false otherwise
*/
bool snake_collision(snake_t* snake1, snake_t* snake2) {
    uint8_t i = 1;
    for (i = 1; i < snake2->cur_length; i++) {
        if (collision(snake1, snake2->tail[i])) {
            return true;
        }
    }
    return false;
}


