/*
 *      Comunicação UART via DMA - DMA_USART.c
 *
 *      Data: 27 de julho, 2023
 *      Autor: Gabriel Luiz
 *      Contato: (31) 97136-4334 || gabrielluiz.eletro@gmail.com
 */
/*
 * 		- Links Úteis -
 *
 *      DATASHEET:
 */

#include "DMA_USART.h"

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

extern UART_HandleTypeDef huart2; /* */
extern UART_HandleTypeDef huart3; /* */

/* USER CODE END EV */

/* External functions ------------------------------------------------------------*/
/* USER CODE BEGIN EF */

extern void Error_Handler(); /* */
extern void USART3_Message_Received();
/* USER CODE END EF */

/* Private variables --------------------------------------------------------*/
/* USER CODE BEGIN PV */

uint8_t DMA_RX_Buffer_3[DMA_RX_BUFFER_SIZE]; /* */
uint8_t DMA_RX_Buffer_2[DMA_RX_BUFFER_SIZE]; /* */

/* USER CODE END PV */

/* Private functions ------------------------------------------------------------*/
/* USER CODE BEGIN PF */

/**
 * @brief
 * @param
 * @param
 * @retval ***NONE***
 */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	/* Prevent unused argument(s) compilation warning */
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
	if (huart->Instance == USART3) {
		USART3_Message_Received();
		HAL_UARTEx_ReceiveToIdle_DMA(&huart3, DMA_RX_Buffer_3,
				DMA_RX_BUFFER_SIZE);
	}
	if (huart->Instance == USART2) {
		uart2MessageReceived();
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, DMA_RX_Buffer_2,
				DMA_RX_BUFFER_SIZE);
	}
}

/**
 * @brief
 * @param
 * @param
 * @retval ***NONE***
 */
void USART_Init(void) {
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, DMA_RX_Buffer_3, DMA_RX_BUFFER_SIZE);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, DMA_RX_Buffer_2, DMA_RX_BUFFER_SIZE);
}

/* USER CODE END PF */
