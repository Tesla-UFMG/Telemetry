/*
 * DMA_USART.h
 *
 *  Created on: 28 de ago de 2019
 *      Author: Guilherme Amorim
 */

#ifndef DMA_USART_H_
#define DMA_USART_H_


#include "stm32f1xx_hal.h"
#include <string.h>

#define DMA_RX_BUFFER_SIZE 128
#define DMA_RX_BUFFER_SIZE_NEXTION 32
#define DMA_RX_BUFFER_SIZE_XBEE 64

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void USART_Init(void);

#endif /* DMA_USART_H_ */
