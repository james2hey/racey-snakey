/** @file game.c
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 14/10/2017
 *  @brief Controls the main game loop
*/

#include "game.h"

/** Define frequency of tasks*/
#define DISPLAY_TASK_RATE 2200
#define CONTROL_TASK_RATE 70
#define UPDATE_TASK_RATE 1

/** Define the starting position of the snakes */
#define SNAKE1_X 0
#define SNAKE1_Y 6
#define SNAKE1_DIR UP
#define SNAKE2_X 4
#define SNAKE2_Y 0
#define SNAKE2_DIR DOWN
#define SNAKE_LENGTH 2

/**Define the colour of the opponent's snake (the proportion of time the
 * snake's LEDs are on is given by OPP_SNAKE_COL / COLOUR_STEPS)*/
#define COLOUR_STEPS 3
#define OPP_SNAKE_COL 1

/**Structure to hold game data*/
typedef struct game_data_s {
    snake_t snake1;
    snake_t snake2;
    tinygl_point_t food;
    uint8_t player_num;
} game_data_t;


/** Draws the static objects onto the LED matrix i.e. objects which only
 *  need to be redrawn when the game is updated. These objects are the
 *  player's snake and the food.
 *  @param data game data, the snake and food positions
*/
static void draw_static(game_data_t* game_data)
{
    tinygl_clear();
    tinygl_draw_point(game_data->food, 1);
    snake_draw(&game_data->snake1, 1);
}


/** Draws the dynamic objects onto the LED matrix i.e. objects which
 *  need to be redrawn on every refresh of the LED matrix. This is the
 *  opponent's snake, since it flickers rapidly to give a darker colour.
 *  @param data game data, the snake and food positions
*/
static void draw_dynamic(game_data_t* game_data)
{
    static uint8_t colour_tick = 0;
    if (colour_tick == 0 || colour_tick == OPP_SNAKE_COL) {
        snake_draw(&game_data->snake2, colour_tick < OPP_SNAKE_COL);
    }
    colour_tick = (colour_tick + 1) % COLOUR_STEPS;
}


/** Updates the LED matrix
 *  @param data game data, the snake and food positions
*/
static void display_task(void* data)
{
    static uint8_t col = 0;
    game_data_t* game_data = data;
    if (col == 0) {
        draw_dynamic(game_data);
    }
    tinygl_update();
    col = (col + 1) % TINYGL_WIDTH;
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
    
    if (game_data->snake1.alive) { 
        tinygl_clear();
        tinygl_update();
        navswitch_update();
        draw_static(game_data);
        display_task(data);
    
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


/** Sends data about the player snake's position to the opponent, or
 *  receives data about the opponents snake.
 *  @param game_data game data, the snake and food positions
 *  @param sender indicates whether player 1 or 2 is sending (and so
 *  the other player is receiving.
*/
static void send_snake(game_data_t* game_data, uint8_t sender)
{   
    if (game_data->player_num == sender) {
        send_val(game_data->snake1.dir);
    } else {
        game_data->snake2.dir = receive_val(UP, LEFT);
    }
    
    if (game_data->player_num == sender) {
        send_val(game_data->snake1.alive);
    } else {
        game_data->snake2.alive = receive_val(0, 1);
    }
}


/** Clears the screen and ends the task loop so that it returns the
 *  game result.
 *  @param game_result indicates the winning player (or if it was a
 *  draw).
*/
static void end_game(uint8_t game_result)
{
    tinygl_clear();
    tinygl_update();
    task_cancel(game_result);
}


/** Moves the food to a new position
 *  @param game_data game data, the snake and food positions
*/
static void make_new_food(game_data_t* game_data)
{
    tinygl_point_t forbidden[2 * MAX_SNAKE_LENGTH];
    uint8_t length1 = game_data->snake1.cur_length;
    uint8_t length2 = game_data->snake2.cur_length;
    uint8_t i = 0;
    for (i = 0; i < length1; i++) {
        forbidden[i] = game_data->snake1.tail[i];
    }
    for (i = 0; i < length2; i++) {
        forbidden[i + length1] = game_data->snake2.tail[i];
    }
    
    game_data->food = new_food(length1 + length2, forbidden);
}


/** Updates the game state by moving the snakes, checking if
 *  have collided or eaten food
 *  @param data game data, the snake and food positions
*/
static void update_task(void* data)
{
    game_data_t* game_data = data;
    
    send_snake(game_data, 1);
    send_snake(game_data, 2);
          
    if (game_data->snake1.alive && game_data->snake2.alive) {
        snake_move(&game_data->snake1);
        snake_move(&game_data->snake2);
        
        if (snake_collision(&game_data->snake1, &game_data->snake1) ||
            snake_collision(&game_data->snake1, &game_data->snake2) ||
            collision(&game_data->snake1, game_data->snake2.tail[0])) {
            game_data->snake1.alive = false;
        }
        
        if (collision(&game_data->snake1, game_data->food)) {
            snake_eat(&game_data->snake1);
            make_new_food(game_data);
            send_coord(game_data->food);
        } else if (collision(&game_data->snake2, game_data->food)) {
            snake_eat(&game_data->snake2);
            game_data->food = receive_coord();
        }
    } else {
        if (!game_data->snake1.alive) {
            snake_move(&game_data->snake1);
        }
        if (!game_data->snake2.alive) {
            snake_move(&game_data->snake2);
        }
        
        if (game_data->snake1.cur_length == 0 && game_data->snake2.cur_length == 0) {
            end_game(DRAW);
        } else if(game_data->snake1.cur_length == 0) {
            end_game(LOSE);
        } else if(game_data->snake2.cur_length == 0) {
            end_game(WIN);
        }
        
    }
    
    draw_static(game_data);
    display_task(data);
}


/** Begins the snake game 
 *  @param player_n indicates whether the funkit is player 1 or 2
 *  @return an integer indicating the result of the game
*/
uint8_t begin_game(uint8_t player_n)
{
    game_data_t game_data;
    game_data.player_num = player_n;
    
    if (game_data.player_num == 1) {
        game_data.snake1 = create_snake(SNAKE1_X, SNAKE1_Y, SNAKE_LENGTH, SNAKE1_DIR);
        game_data.snake2 = create_snake(SNAKE2_X, SNAKE2_Y, SNAKE_LENGTH, SNAKE2_DIR);
    } else {
        game_data.snake1 = create_snake(SNAKE2_X, SNAKE2_Y, SNAKE_LENGTH, SNAKE2_DIR);
        game_data.snake2 = create_snake(SNAKE1_X, SNAKE1_Y, SNAKE_LENGTH, SNAKE1_DIR);
    }
    
    game_data.food.x = 2;
    game_data.food.y = 3;
    
    task_t tasks[] = {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE, .data = &game_data},
        {.func = control_task, .period = TASK_RATE / CONTROL_TASK_RATE, .data = &game_data},
        {.func = update_task, .period = TASK_RATE / UPDATE_TASK_RATE, .data = &game_data},
    };
    
    return task_schedule(tasks, ARRAY_SIZE (tasks));
}
