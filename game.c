/** @file game.c
 *  @author Gerry Toft, 53712395
 *  @date 14/10/2017
 *  @brief Controls the main game loop
*/

#include "game.h"

/** Define frequency of tasks*/
#define DISPLAY_TASK_RATE 1400
#define CONTROL_TASK_RATE 70
#define UPDATE_TASK_RATE 5

/**Structure to hold game data*/
typedef struct game_data_s {
    snake_t snake1;
    tinygl_point_t food;
    bool running;
} game_data_t;

/** Draw the snakes and food onto the LED matrix 
 *  @param data game data, the snake and food positions
*/
static void display_task(void* data)
{
    game_data_t* game_data = data;
    
    tinygl_clear();
    snake_draw(&game_data->snake1);
    
    if (game_data->running) {
        tinygl_draw_point(game_data->food, 1);
    }
    
    tinygl_update();
}

/** Checks if a move in the given direction is a valid move
 *  (i.e. isn't a move back onto the snake's tail)
 *  @param dir the direction to move in
 *  @param snake the snake which is being moved
 *  @return true if the move is valid, false otherwise
*/
static bool dir_okay(uint8_t dir, snake_t* snake)
{
    if (snake->cur_length == 1) {
        return true;
    } else {
        tinygl_point_t head_posn = new_head_posn(dir, snake);
        return (head_posn.x != snake->tail[1].x || head_posn.y != snake->tail[1].y);
    } 
}

/** Reads navswitch input and sets the snake to move in the
 *  corresponding direction.
 *  @param data game data, the snake and food positions
*/
static void control_task(void* data)
{
    game_data_t* game_data = data;
    
    if (game_data->running) { 
        tinygl_clear();
        tinygl_update();
        navswitch_update();
    
        if (navswitch_push_event_p(NAVSWITCH_NORTH) && dir_okay(UP, &game_data->snake1)) {
            game_data->snake1.dir = UP;
        } else if (navswitch_push_event_p(NAVSWITCH_SOUTH) && dir_okay(DOWN, &game_data->snake1)) {
            game_data->snake1.dir = DOWN;
        } else if (navswitch_push_event_p(NAVSWITCH_WEST) && dir_okay(LEFT, &game_data->snake1)) {
            game_data->snake1.dir = LEFT;
        } else if (navswitch_push_event_p(NAVSWITCH_EAST) && dir_okay(RIGHT, &game_data->snake1)) {
            game_data->snake1.dir = RIGHT;
        }
    }
}

/** Updates the game state by moving the snakes, checking if
 *  have collided or eaten food
 *  @param data game data, the snake and food positions
*/
static void update_task(void* data)
{
    game_data_t* game_data = data;
    
    if (game_data->running) {
        snake_move(&game_data->snake1);
    } else if (game_data->snake1.cur_length > 0) {
        game_data->snake1.cur_length--;
    } else {
        //end the game blah i dont know how to do this
        return;
    }
    
    if (snake_collision(&game_data->snake1, &game_data->snake1)) {
        game_data->running = false;
    }
    
    if (collision(&game_data->snake1, game_data->food)) {
        snake_eat(&game_data->snake1);
        game_data->food = new_food(game_data->snake1.cur_length, game_data->snake1.tail);
    }
}

/** Begins the snake game */
void begin_game(void)
{
    game_data_t game_data;
    game_data.snake1 = create_snake(0, 6, 2, UP);
    game_data.food.x = 2;
    game_data.food.y = 3;
    game_data.running = true;
    
    task_t tasks[] = {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE, .data = &game_data},
        {.func = control_task, .period = TASK_RATE / CONTROL_TASK_RATE, .data = &game_data},
        {.func = update_task, .period = TASK_RATE / UPDATE_TASK_RATE, .data = &game_data}
    };
    
    task_schedule(tasks, ARRAY_SIZE (tasks));
}
