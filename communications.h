#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "system.h"
#include "timer.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "snake.h"

#define READ_TIMEOUT 2000

void receive_snake(snake_t* snake_buffer);

void send_snake(snake_t* snake);

#endif
