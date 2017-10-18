#include "communications.h"

void send_val(uint8_t val) {
    uint8_t ack = 0;
    while (ack != ACK_NO) {
        if (ir_uart_read_ready_p()) {
            ack = ir_uart_getc();
        } else {
            ir_uart_putc(val + MAGIC_NO);
            pacer_wait();
        }
    }
}

uint8_t receive_val(uint8_t min, uint8_t max) {
    bool received = false;
    uint8_t read = 0;
    while (!received) {
        if (ir_uart_read_ready_p()) {
            read = ir_uart_getc() - MAGIC_NO;
            if (read >= min && read <= max) {
                ir_uart_putc(ACK_NO);
                received = true;
            }
        }
        
        if (!received) {
            pacer_wait();
        }
    }
    return read;
}
/*
void send_val(uint8_t val) {
    ir_uart_putc(val + MAGIC_NO);
}

uint8_t receive_val(uint8_t min, uint8_t max) {
    uint8_t read = 0;
    read = ir_uart_getc() - MAGIC_NO;
    while (read < min && read > max) {
        read = ir_uart_getc() - MAGIC_NO;
    }
    return read;
}
*/
void send_coord(tinygl_point_t point) {
    uint8_t to_send = TINYGL_WIDTH * point.y + point.x;
    send_val(to_send);
}

tinygl_point_t receive_coord(void) {
    tinygl_point_t coord;
    uint8_t read = receive_val(0, TINYGL_WIDTH * TINYGL_HEIGHT - 1);
    coord.x = read % TINYGL_WIDTH;
    coord.y = read / TINYGL_WIDTH;
    return coord;
}

