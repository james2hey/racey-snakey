#include "communications.h"


void receive_snake(snake_t* snake_buffer) {
    timer_tick_t now = timer_get();
    timer_tick_t end_time = now + READ_TIMEOUT * TIMER_RATE;
    char read;
    uint8_t num_read = 0;
    
    while (end_time - now < TIMER_OVERRUN_MAX && num_read < MAX_SNAKE_LENGTH) {
        if (ir_uart_read_ready_p()) {
            read = ir_uart_getc();
            snake_buffer->tail[num_read].x = read % TINYGL_WIDTH;
            snake_buffer->tail[num_read].y = read / TINYGL_WIDTH;
            num_read++;
        }
    }
    
    snake_buffer->cur_length = num_read;
}


void send_snake(snake_t* snake) {
    char snake_string[snake->cur_length + 1];
    uint8_t i = 0;
    for (i = 0; i < snake->cur_length; i++) {
        snake_string[i] = TINYGL_WIDTH * snake->tail[i].y + snake->tail[i].x;
    }
    snake_string[snake->cur_length] = '\0';
    ir_uart_puts(snake_string);
}
