/*
 * xbee.h
 *
 *  Created on: 8 de ago de 2019
 *      Author: Formula Tesla UFMG
 */

#ifndef XBEE_H_
#define XBEE_H_

#include <stdarg.h>
#include "stm32f1xx_hal.h"
#include "usart.h"
#include <string.h>

/*Defines*/
#define DELAY_XBEE 0

/*XBEE MODE ENUM*/
typedef enum {
  BYTES_API,
  STRING_API,
  STRING_TRANSPARENT,
  BYTES_TRANSPARENT
}SendMode_e;

/* Functions */
void xbeeInit(SendMode_e init_mode);
uint8_t xbeeApiModeSend(char *buff_message, int buff_size);
void xbeeSend(int id, ...);

#endif /* XBEE_H_ */
