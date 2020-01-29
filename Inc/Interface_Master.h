/*
 * Interface_Master.h
 *
 *  Created on: September 15 2019
 *      Author: Formula Tesla UFMG
 *  
 * 
 *  Identificadores Rede CAN: https://docs.google.com/spreadsheets/d/1wuYIXwkoI3ViJUeTy1g0zmYpwM5OYqyshXtDRXcwHPk/edit#gid=1354529818
 * 
 */

#ifndef INTERFACE_MASTER_H_
#define INTERFACE_MASTER_H_

/* Includes */
#include "stm32f1xx_hal.h"
#include "DMA_USART.h"
#include "nextion.h"
#include "xbee.h"
#include "can.h"

/* Defines */
#define CAN_IDS_NUMBER 360
#define REAL_CLK_CAN_ID 10

/* TADs */

/* CAN message struct */
typedef struct{
  uint16_t word_0;
  uint16_t word_1;
  uint16_t word_2;
  uint16_t word_3;
}CanIdData_t;

/* Timer struct */
typedef struct{
  uint32_t previous;
  uint16_t interval;
}Timer_t;

/*Functions*/

/* Other Functions */
void UART_Print_Debug(char* format, ...);
void interfaceInit(void);
void canMessageReceived(uint16_t id, uint8_t* data);
void blinkLed1(void);
void blinkLed2(void);
void blinkLed3(void);
void timerAtualization(void);
void debugFunction(void);

#endif  /* INTERFACE_MASTER_H_ */
