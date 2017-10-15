/** @file communications.h
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 14/10/2017
 *  @brief Functions for sending data between the funkits.
*/

#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "system.h"
#include "timer.h"
#include "ir_uart.h"
#include "tinygl.h"

/** Sends a single integer across the IR transmitter
 *  @param val the integer to transmit
*/
void send_val(uint8_t val);

uint8_t receive_val(void);

void send_coord(tinygl_point_t point);

tinygl_point_t receive_coord(void);

#endif
