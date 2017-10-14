#include "communications.h"


void receive_snake(snake_t* snake_buffer) {
    timer_tick_t now = timer_get();
    timer_tick_t end_time = now + READ_TIMEOUT * TIMER_RATE;
    char read;
    uint8_t num_read = 0;
    uint8_t snake_length = MAX_SNAKE_LENGTH + 1;
    
    while (end_time - now < TIMER_OVERRUN_MAX && num_read < snake_length) {
        if (ir_uart_read_ready_p()) {
            read = ir_uart_getc();
            if (snake_length > MAX_SNAKE_LENGTH) {
                if (read >= 0 && read <= MAX_SNAKE_LENGTH) {
                    snake_length = read;
                }
            } else if (read >= 0 && read <= TINYGL_WIDTH * TINYGL_HEIGHT - 1) {
                snake_buffer->tail[num_read].x = read % TINYGL_WIDTH;
                snake_buffer->tail[num_read].y = read / TINYGL_WIDTH;
                num_read++;
            }
        }
    }
    
    if (num_read == snake_length) {
        snake_buffer->cur_length = snake_length;
    } else {
        snake_buffer->cur_length = 0;
    }
}


void send_snake(snake_t* snake) {
    char snake_string[snake->cur_length + 2];
    snake_string[0] = snake->cur_length;
    uint8_t i = 0;
    for (i = 0; i < snake->cur_length; i++) {
        snake_string[i + 1] = TINYGL_WIDTH * snake->tail[i].y + snake->tail[i].x;
    }
    snake_string[snake->cur_length] = '\0';
    ir_uart_puts(snake_string);
}
