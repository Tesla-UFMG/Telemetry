/*
 * Interface_Master.c
 *
 *  Created on: Semptember 15 2019
 *      Author: Formula Tesla UFMG
 *  
 * 
 *  Identificadores Rede CAN: https://docs.google.com/spreadsheets/d/1wuYIXwkoI3ViJUeTy1g0zmYpwM5OYqyshXtDRXcwHPk/edit#gid=1354529818
 * 
 */

#include <Interface_Master.h>

/*Timers variables*/
Timer_t packTimer;
uint32_t actualTimer;

/* CAN data struct vector */
CanIdData_t can_vector[CAN_IDS_NUMBER];

/* DMA and UARTs structs */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;

/* Auxiliar Variables */
int auxiliar = 0;
uint8_t vet_aux[128];


/* Functions */

void interfaceInit(void)
{
  /* UART DMA Init */
  USART_DMA_Init(&huart2, &hdma_usart2_rx);
  USART_DMA_Init(&huart3, &hdma_usart3_rx);

  /* CAN Init */
  CAN_Config_Filter();
  CAN_Config_Frames();
  CAN_Receive_IT();

  /* can data vector init */
  for(uint16_t i = 0; i < CAN_IDS_NUMBER; i++){
    can_vector[i].word_0 = 0;
    can_vector[i].word_1 = 0;
    can_vector[i].word_2 = 0;
    can_vector[i].word_3 = 0;
  }

	blinkLed1();
	blinkLed2();
	blinkLed3();

  //xbeeInit(BYTES_API);

  /* Requesting real time */
  HAL_Delay(1000);
  //realClockRequest(); /* 2s Delay */

  /* Nextion Init */
  nexInit();

  /* Global timer variables init */
  packTimer.previous = HAL_GetTick();
  packTimer.interval = 1000;
}

void UART_Print_Debug(char* format, ...)
{
	char buffer[100];
	uint8_t size = 0;
	buffer[0] = '\0';

	va_list argList;

	va_start(argList, format);
	size = vsprintf(buffer, format, argList);

	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, size, 100);

	va_end(argList);
}

void canMessageReceived(uint16_t id, uint8_t* data)
{
	if(id > CAN_IDS_NUMBER - 1)	return;
  blinkLed1();
  uint16_t* data_word = (uint16_t*)data;
  can_vector[id].word_0 = data_word[0];
  can_vector[id].word_1 = data_word[1];
  can_vector[id].word_2 = data_word[2];
  can_vector[id].word_3 = data_word[3];
}

void blinkLed1(void)
{
	HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
} /* Debug Led 1 */

void blinkLed2(void)
{
	HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
} /* Debug Led 2 */

void blinkLed3(void)
{
	HAL_GPIO_TogglePin(LED_3_GPIO_Port, LED_3_Pin);
} /* Debug Led 3 */

void timerAtualization(void)
{
  actualTimer = HAL_GetTick();
}

void debugFunction(void)
{ 
  // auxiliar++;
  // if(auxiliar == 100) auxiliar = 0;
}
