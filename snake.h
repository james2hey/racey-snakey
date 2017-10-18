/** @file snake.h
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 12/10/2017
 *  @brief Defines a snake structure and functions for controlling it.
*/

#ifndef SNAKE_H
#define SNAKE_H

#include "system.h"
#include "tinygl.h"

/** The maximum length of a snake */
#define MAX_SNAKE_LENGTH 8

/** Defines constants for the directions of the snakes movement */
enum directions {UP, RIGHT, DOWN, LEFT};

/** Defines a snake as the points in its tail, its length, and the
 *  direction of its movements
*/
typedef struct snake_s {
    uint8_t cur_length;
    uint8_t length;
    uint8_t dir;
    bool alive;
    tinygl_point_t tail[MAX_SNAKE_LENGTH];
} snake_t;

/** Creates a new snake structure from given initial properties
 *  @param x the x coordinate the snake starts at
 *  @param y the y coordinate the snake starts at
 *  @param length the initial length of the snake
 *  @param dir the snakes initial direction of motion
 *  @return the snake structure
*/
snake_t create_snake(uint8_t x, uint8_t y, uint8_t length, uint8_t dir);

/** Function to be called when a snake eats food. This increases\
 *  the snake's length up to the maximum length
 *  @param snake the snake which has eaten food
*/
void snake_eat(snake_t* snake);

/** Calculates the position the snake's head will be at after
 *  moving in the given direction
 *  @param dir the direction the snake will move in
 *  @param snake the snake that is moving
 *  @return the new position of the head
*/
tinygl_point_t new_head_posn(uint8_t dir, snake_t* snake);

/** Updates the position of the snake and its tail. The snake will wrap
 *  through walls.
 *  @param snake the snake to move
*/
void snake_move(snake_t* snake);

/** Draws the given snake onto the LED matrix
 *  @param snake the snake to draw
*/
void snake_draw(snake_t* snake, uint8_t val);

/** Detects if the snake's head has collided with the given point
 *  @param snake the snake
 *  @param point the point to check for collision
 *  @return returns true if there is a collision, false otherwise
*/
bool collision(snake_t* snake, tinygl_point_t point);

/** Detects if a snake has collided with the tail of another. If
 *  snake1 = snake2 the function detects if snake1 has collided
 *  with itself.
 *  @param snake1 the snake who's head is checked for collision
 *  @param snake2 the snake who's tail is checked for collision
 *  @return returns true if the snakes have collided, false otherwise
*/
bool snake_collision(snake_t* snake1, snake_t* snake2);

#endif
