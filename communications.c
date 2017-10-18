/** @file communications.c
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 14/10/2017
 *  @brief Functions for sending data between the funkits.
*/
#include "communications.h"


/** Sends a value across IR transmission
 *  @param val the value to send
*/
void send_val(uint8_t val) 
{
    ir_uart_putc(val);
}


/** Receives a value from IR receiver
 *  @param min the minimum value to accept
 *  @param max the maximum value to accept
 *  @return the received value
*/
uint8_t receive_val() 
{
    return ir_uart_getc();
}


/** Sends a coordinate across IR transmission
 *  @param point the coordinate to send
*/
void send_coord(tinygl_point_t point) 
{
    uint8_t to_send = TINYGL_WIDTH * point.y + point.x;
    send_val(to_send);
}


/** Receives a coordinate from IR receiver
 *  @return the received coordinate
*/
tinygl_point_t receive_coord(void) 
{
    tinygl_point_t coord;
    uint8_t read = receive_val();
    coord.x = read % TINYGL_WIDTH;
    coord.y = read / TINYGL_WIDTH;
    return coord;
}

