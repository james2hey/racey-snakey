#include "communications.h"

void send_val(uint8_t val) {
    ir_uart_putc(val);
}

uint8_t receive_val(void) {
    return ir_uart_getc();
}

void send_coord(tinygl_point_t point) {
    uint8_t to_send = TINYGL_WIDTH * point.y + point.x;
    ir_uart_putc(to_send);
}

tinygl_point_t receive_coord(void) {
    tinygl_point_t result;
    uint8_t read = ir_uart_getc();
    result.x = read % TINYGL_WIDTH;
    result.y = read / TINYGL_WIDTH;
    return result;
}
