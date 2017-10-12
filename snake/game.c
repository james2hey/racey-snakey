#include "system.h"
#include "tinygl.h"
#include "task.h"
#include "navswitch.h"
#include "snake.h"

#define DISPLAY_TASK_RATE 2000
#define CONTROL_TASK_RATE 100
#define UPDATE_TASK_RATE 10

void draw_points(tinygl_point_t points[], uint8_t length) {
    uint8_t i = 0;
    
    tinygl_clear();
    for (i = 0; i < length; i++) {
        tinygl_draw_point(points[i], 1);
    }
}


static void display_task_init(void) {
    tinygl_init(DISPLAY_TASK_RATE);
}


static void display_task(void* data) {
    snake_t* snake = data;
    
    draw_points(snake->tail, snake->cur_length);
    tinygl_update();
}

static void control_task_init(void) {
    navswitch_init();
}

static void control_task(void* data) {
    snake_t* snake = data;
    
    navswitch_update();
    
    if (navswitch_push_event_p(NAVSWITCH_NORTH) && snake->dir != DOWN) {
        snake->dir = UP;
    } else if (navswitch_push_event_p(NAVSWITCH_SOUTH) && snake->dir != UP) {
        snake->dir = DOWN;
    } else if (navswitch_push_event_p(NAVSWITCH_WEST) && snake->dir != RIGHT) {
        snake->dir = LEFT;
    } else if (navswitch_push_event_p(NAVSWITCH_EAST) && snake->dir != LEFT) {
        snake->dir = RIGHT;
    }
}

static void update_task(void* data) {
    snake_t* snake = data;
    snake_move(snake);
}


int main(void) {
    snake_t snake = create_snake(1, 1);
    
    task_t tasks[] = {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE, .data = &snake},
        {.func = control_task, .period = TASK_RATE / CONTROL_TASK_RATE, .data = &snake},
        {.func = update_task, .period = TASK_RATE / UPDATE_TASK_RATE, .data = &snake}
    };
    
    display_task_init();
    control_task_init();
    
    task_schedule(tasks, ARRAY_SIZE (tasks));
    return 0;
}
