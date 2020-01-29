/*
 * DMA_USART.c
 *
 *  Created on: 28 de ago de 2019
 *      Author: Guilherme Amorim
 */

#include <DMA_USART.h>

uint8_t uart_user_message[DMA_RX_BUFFER_SIZE];	/* Buffer received for user access */
uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];	/* Local DMA buffer for circular DMA */

void USART_DMA_Init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma)
{
	__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);   // enable idle line interrupt
	__HAL_DMA_ENABLE_IT(hdma, DMA_IT_TC);  // enable DMA Tx cplt interrupt
	__HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT); 	// discable half complete interrupt
	HAL_UART_Receive_DMA(huart, DMA_RX_Buffer, DMA_RX_BUFFER_SIZE);
}

void USART_IrqHandler (UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma)
{
	if (huart->Instance->SR & UART_FLAG_IDLE)           /* if Idle flag is set */
	{
		volatile uint32_t tmp;							/* Must be volatile to prevent optimizations */
        tmp = huart->Instance->SR;                      /* Read status register */
        tmp = huart->Instance->DR;                      /* Read data register */
    	__HAL_DMA_DISABLE(hdma);       					/* Disabling DMA will force transfer complete interrupt if enabled */
		DMA_IrqHandler(hdma, huart);
	}
}

void DMA_IrqHandler (DMA_HandleTypeDef *hdma, UART_HandleTypeDef *huart)
{	
	uint16_t len;	/* To store UART Messade received len */

	DMA_Base_Registers *regs = (DMA_Base_Registers*)hdma->DmaBaseAddress;
	
	if(__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_TC) != RESET)	/* if the source is TC */
	{
		/* Clear the transfer complete flag */
		__HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));

		/* Clear the uart_user_message buffer */
		memset(uart_user_message, 0, DMA_RX_BUFFER_SIZE);

		/* Copying the message to uart_user_message buffer	*/
		len = DMA_RX_BUFFER_SIZE - hdma->Instance->CNDTR;
		if(len > DMA_RX_BUFFER_SIZE - 1)	return;
		memcpy(uart_user_message, DMA_RX_Buffer, len);

		/* Uncomment below to transmit the data via uart */
		//HAL_UART_Transmit(huart, uart_user_message, len, 100);
		
		/* Prepare DMA for next transfer */
        /* Important! DMA stream won't start if all flags are not cleared first */
 
        regs->IFCR = 0x3FU << hdma->ChannelIndex; 			/* Clear all interrupts */
		hdma->Instance->CMAR = (uint32_t)DMA_RX_Buffer;   	/* Set memory address for DMA again */
        hdma->Instance->CNDTR = DMA_RX_BUFFER_SIZE;    		/* Set number of bytes to receive */
        hdma->Instance->CCR |= DMA_CCR_EN;            		/* Start DMA transfer */
	}
}
