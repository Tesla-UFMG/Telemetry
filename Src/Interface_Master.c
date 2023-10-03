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
#include "lcd_dash.h"
#include <stdio.h>

/*Timers variables*/
uint32_t actualTimer;
extern uint8_t pageMessageReceived;

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

void interfaceInit(void) {
	HAL_Delay(100);

	USART_Init();

	nexInit();

	NEXTION_Init();

	/* Global timer variables init */
}

void UART_Print_Debug(char *format, ...) {
	char buffer[100];
	uint8_t size = 0;
	buffer[0] = '\0';

	va_list argList;

	va_start(argList, format);
	size = vsprintf(buffer, format, argList);

	HAL_UART_Transmit(&huart1, (uint8_t*) buffer, size, 100);

	va_end(argList);
}

void canMessageReceived(uint16_t id, uint8_t *data) {
	if (id > CAN_IDS_NUMBER - 1)
		return;
	uint16_t *data_word = (uint16_t*) data;
	can_vector[id].word_0 = data_word[0];
	can_vector[id].word_1 = data_word[1];
	can_vector[id].word_2 = data_word[2];
	can_vector[id].word_3 = data_word[3];
}

void blinkLed1(void) {
	HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
} /* Debug Led 1 */

void blinkLed2(void) {
	HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
} /* Debug Led 2 */

void blinkLed3(void) {
	HAL_GPIO_TogglePin(LED_3_GPIO_Port, LED_3_Pin);
} /* Debug Led 3 */

void timerAtualization(void) {
	actualTimer = HAL_GetTick();
}

void debugFunction(void) {
	// auxiliar++;
	// if(auxiliar == 100) auxiliar = 0;
}

//timer_handler - BMS

uint8_t timer_wait_ms(uint32_t timer_start, uint32_t delay) {
	const uint32_t current_time = HAL_GetTick();
	if ((current_time - timer_start) >= delay) {
		return 1;
	}
	return 0;
}
void timer_restart(uint32_t *timer_to_restart) {
	*timer_to_restart = HAL_GetTick();
}
