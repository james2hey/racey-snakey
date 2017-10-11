#include "system.h"
#include "tinygl.h"
#include "task.h"

#define DISPLAY_RATE 2000
void draw_points(tinygl_point_t points[], uint8_t length) {
    uint8_t i = 0;
    
    tinygl_clear();
    for (i = 0; i < length; i++) {
        tinygl_draw_point(points[i], 1);
    }
}


static void display_points_init(void) {
    tinygl_init(DISPLAY_RATE);
}


static void display_points_task(void* data) {
    tinygl_point_t* points = data;
    
    draw_points(points, 3);
    tinygl_update();
}


int main(void) {
    tinygl_point_t points[] = {
        {.x = 1, .y = 1}, {.x = 2, .y = 2}, {.x = 3, .y = 3}
    };
    
    task_t tasks[] = {
        {.func = display_points_task, .period = TASK_RATE / DISPLAY_RATE, .data = points}
    };
    
    display_points_init();
    
    task_schedule(tasks, ARRAY_SIZE (tasks));
    return 0;
}
