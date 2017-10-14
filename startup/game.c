#include "game.h"

#define DISPLAY_TASK_RATE 1400
#define CONTROL_TASK_RATE 70
#define UPDATE_TASK_RATE 5

typedef struct game_data_s {
    snake_t snake1;
    tinygl_point_t food;
} game_data_t;


static void display_task(void* data) {
    game_data_t* game_data = data;
    
    tinygl_clear();
    snake_draw(&game_data->snake1);
    tinygl_draw_point(game_data->food, 1);
    tinygl_update();
}

static bool dir_okay(uint8_t dir, snake_t* snake) {
    if (snake->cur_length == 1) {
        return true;
    } else {
        tinygl_point_t head_posn = new_head_posn(dir, snake);
        return (head_posn.x != snake->tail[1].x || head_posn.y != snake->tail[1].y);
    } 
}

static void control_task(void* data) {
    game_data_t* game_data = data;
    
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

static void update_task(void* data) {
    game_data_t* game_data = data;
    snake_move(&game_data->snake1);
}


void begin_game(void) {
    game_data_t game_data;
    game_data.snake1 = create_snake(0, 6);
    game_data.food = new_food(game_data.snake1.cur_length, game_data.snake1.tail);
    //game_data.food.x = 2;
    //game_data.food.y = 3;
    
    task_t tasks[] = {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE, .data = &game_data},
        {.func = control_task, .period = TASK_RATE / CONTROL_TASK_RATE, .data = &game_data},
        {.func = update_task, .period = TASK_RATE / UPDATE_TASK_RATE, .data = &game_data}
    };
    
    
    
    task_schedule(tasks, ARRAY_SIZE (tasks));
}
