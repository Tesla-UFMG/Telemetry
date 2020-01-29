/*
 * DMA_USART.h
 *
 *  Created on: 28 de ago de 2019
 *      Author: Guilherme Amorim
 */

#ifndef DMA_USART_H_
#define DMA_USART_H_

/*
 * If you want to transmit the received data uncomment line 52
*/

#include "stm32f1xx_hal.h"
#include <string.h>

#define DMA_RX_BUFFER_SIZE 256

typedef struct
{
    __IO uint32_t ISR;          /*!< DMA interrupt status register */
    __IO uint32_t Reserved0;
    __IO uint32_t IFCR;         /*!< DMA interrupt flag clear register */
}DMA_Base_Registers;

void USART_DMA_Init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma);
void USART_IrqHandler(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma);
void DMA_IrqHandler(DMA_HandleTypeDef *hdma, UART_HandleTypeDef *huart);

#endif /* DMA_USART_H_ */
