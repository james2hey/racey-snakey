#include "system.h"
#include "tinygl.h"
#include "task.h"
#include "navswitch.h"
#include "snake.h"

#define DISPLAY_TASK_RATE 5000
#define CONTROL_TASK_RATE 70
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

static bool dir_okay(uint8_t dir, snake_t* snake) {
    if (snake->cur_length == 1) {
        return true;
    } else {
        tinygl_point_t head_posn = new_head_posn(dir, snake);
        return (head_posn.x != snake->tail[1].x || head_posn.y != snake->tail[1].y);
    } 
}

static void control_task(void* data) {
    snake_t* snake = data;
    
    tinygl_clear();
    tinygl_update();
    navswitch_update();
    display_task(data);
    
    if (navswitch_push_event_p(NAVSWITCH_NORTH) && dir_okay(UP, snake)) {
        snake->dir = UP;
    } else if (navswitch_push_event_p(NAVSWITCH_SOUTH) && dir_okay(DOWN, snake)) {
        snake->dir = DOWN;
    } else if (navswitch_push_event_p(NAVSWITCH_WEST) && dir_okay(LEFT, snake)) {
        snake->dir = LEFT;
    } else if (navswitch_push_event_p(NAVSWITCH_EAST) && dir_okay(RIGHT, snake)) {
        snake->dir = RIGHT;
    }
}

static void update_task(void* data) {
    snake_t* snake = data;
    snake_move(snake);
}


int main(void) {
    snake_t snake = create_snake(1, 3);
    
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
