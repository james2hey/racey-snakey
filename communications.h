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

/** Defines a magic number which is added to a sent value in the 
 *  hopes of guarding against interference, and an acknowledgement
 *  number to indicate a message was received successfully*/
#define MAGIC_NO 95
#define ACK_NO 64
#define ERROR_NO 27

/** Sends a value across IR transmission
 *  @param val the value to send
*/
void send_val(uint8_t val);

/** Receives a value from IR receiver
 *  @param min the minimum value to accept
 *  @param max the maximum value to accept
 *  @return the received value, or -1 if the read was unsuccessful
*/
int8_t receive_val(uint8_t min, uint8_t max);

void reliable_send_val(uint8_t val);

uint8_t reliable_receive_val(uint8_t min, uint8_t max);

/** Sends a coordinate across IR transmission
 *  @param point the coordinate to send
*/
void send_coord(tinygl_point_t point);

/** Receives a coordinate from IR receiver
 *  @return the received coordinate, which has a negative value of x
 *  if it was received unsuccessfully
*/
tinygl_point_t receive_coord(void);

#endif
