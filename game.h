/** @file game.h
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 14/10/2017
 *  @brief Controls the main game loop
*/

#ifndef GAME_H
#define GAME_H

#include "system.h"
#include "tinygl.h"
#include "mod_task.h"
#include "navswitch.h"
#include "snake.h"
#include "food.h"
#include "communications.h"

enum result {LOSE, WIN, DRAW};

/** Begins the snake game */
uint8_t begin_game(uint8_t playerNum);

#endif
