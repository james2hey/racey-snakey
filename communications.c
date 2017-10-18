/** @file communications.c
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 14/10/2017
 *  @brief Functions for sending data between the funkits.
*/
#include "communications.h"

/** Sends a value across IR transmission
 *  @param val the value to send
*/
void send_val(uint8_t val) {
    ir_uart_putc(val + MAGIC_NO);
}

/** Receives a value from IR receiver
 *  @param min the minimum value to accept
 *  @param max the maximum value to accept
 *  @return the received value, or -1 if the read was unsuccessful
*/
int8_t receive_val(uint8_t min, uint8_t max) {
    uint8_t read = ir_uart_getc() - MAGIC_NO;
    if (read < min || read > max) {
        return -1;
    } else {
        return read;
    }
}

void reliable_send_val(uint8_t val) {
    bool success = false;
    int8_t ack = 0;
    while (!success) {
        send_val(val);
        ack = receive_val(ACK_NO, ACK_NO);
        if (ack == ACK_NO) {
            success = true;
        }
    }
}

uint8_t reliable_receive_val(uint8_t min, uint8_t max) {
    int8_t received = -1;
    while (received < 0) {  
        received = receive_val(min, max);
        if (received == -1) {
            ir_uart_putc(0);
        } else {
            ir_uart_putc(ACK_NO);
        }
    }
    return received;
}

/** Sends a coordinate across IR transmission
 *  @param point the coordinate to send
*/
void send_coord(tinygl_point_t point) {
    uint8_t to_send = TINYGL_WIDTH * point.y + point.x;
    reliable_send_val(to_send);
}

/** Receives a coordinate from IR receiver
 *  @return the received coordinate, which has a negative value of x
 *  if it was received unsuccessfully
*/
tinygl_point_t receive_coord(void) {
    tinygl_point_t coord;
    uint8_t read = reliable_receive_val(0, TINYGL_WIDTH * TINYGL_HEIGHT - 1);
    coord.x = read % TINYGL_WIDTH;
    coord.y = read / TINYGL_WIDTH;
    return coord;
}

