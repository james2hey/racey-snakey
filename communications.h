/** @file communications.h
 *  @author Gerry Toft, 53712395, and James Toohey, 27073776, team 426
 *  @date 14/10/2017
 *  @brief Functions for sending data between the funkits.
*/

#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "system.h"
#include "ir_uart.h"
#include "tinygl.h"

/** Defines a magic number which is added to transmitted values in the
 *  hopes of mitigating interference */
#define MAGIC_NO 93

/** Sends a value across IR transmission
 *  @param val the value to send
*/
void send_val(uint8_t val);

/** Receives a value from IR receiver
 *  @return the received value
*/
uint8_t receive_val(uint8_t min, uint8_t max);

/** Sends a coordinate across IR transmission
 *  @param point the coordinate to send
*/
void send_coord(tinygl_point_t point);

/** Receives a coordinate from IR receiver
 *  @return the received coordinate
*/
tinygl_point_t receive_coord(void);

#endif
