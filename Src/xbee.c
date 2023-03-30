/*
 * xbee.c
 *
 *  Created on: 8 de ago de 2019
 *      Author: Formula Tesla UFMG
 */

#include "xbee.h"

SendMode_e mode = BYTES_API;
uint32_t timer_actual_uart = 0;

uint8_t xbeeApiModeSend(char *xbeeBuffer, int buff_size) {
	/*Send chars through UART (Max 255 chars)*/
	/*Frame with specified address*/
	const char api_start = '\x7E';
	const char api_frame_type = '\x10';
	const char api_frame_id = '\x00'; /* No response is requested */
	const char api_dest_address_64b[9] = "\x00\x13\xA2\x00\x41\x93\x2D\xE3";
	const char api_dest_address_16b[3] = "\x00\x00";
	const char api_broad_radius = '\x00';
	const char api_options = '\x00';
	int checksum;
	int length;

	/*Aux variables*/
	int i, length_0;

	/*If the buff size is larger than 255, return 'fail'*/
	if (buff_size > 255)
		return 0;
	length = buff_size + 14; /*The frame length is 14 bytes + msg size*/

	/*Checksum calc step by step*/
	checksum = api_frame_type + api_frame_id; /*Sum 'type' and 'ID'*/
	for (i = 0; i < 8; i++)
		checksum += api_dest_address_64b[i]; /*Sum the 64b address*/
	for (i = 0; i < 2; i++)
		checksum += api_dest_address_16b[i]; /*Sum the 16b address*/
	checksum += api_broad_radius + api_options; /*Sum 'BroadcastRadius' and 'options'*/
	for (i = 0; i < buff_size; i++)
		checksum += (int) xbeeBuffer[i]; /*Sum the message*/
	checksum = checksum & 0xFF; /*keep only the lowest 8 bits*/
	checksum = 255 - checksum; /*Negate it and get checksum*/

	HAL_UART_Transmit(&huart2, &api_start, 1, 100); /*Send 'start*/
	length_0 = 0xFF00 & length;
	HAL_UART_Transmit(&huart2, &(length_0), 1, 100); /*Send 'length' first byte*/
	length_0 = 0xFF & length;
	HAL_UART_Transmit(&huart2, &(length_0), 1, 100); /*Send 'length' second byte*/
	HAL_UART_Transmit(&huart2, &api_frame_type, 1, 100); /*Send 'freme type'*/
	HAL_UART_Transmit(&huart2, &api_frame_id, 1, 100); /*Send 'freme id'*/
	HAL_UART_Transmit(&huart2, api_dest_address_64b, 8, 100); /*Send '64b address'*/
	HAL_UART_Transmit(&huart2, api_dest_address_16b, 2, 100); /*Send '16b address'*/
	HAL_UART_Transmit(&huart2, &api_broad_radius, 1, 100); /*Send 'freme type'*/
	HAL_UART_Transmit(&huart2, &api_options, 1, 100); /*Send 'options'*/
	HAL_UART_Transmit(&huart2, xbeeBuffer, buff_size, 100); /*Send the message*/
	HAL_UART_Transmit(&huart2, &checksum, 1, 100); /*Send 'checksum'*/
	if (HAL_GetTick() - timer_actual_uart > 60) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
		timer_actual_uart = HAL_GetTick();
	}
	return 1; /*Return 'success'*/
}

//void xbeeInit(SendMode_e init_mode)
//{
////    memset(api_buffer, 0, sizeof(uint8_t)*256);
//
//    switch(init_mode)
//    {
//        case BYTES_API:
//            mode = init_mode;
//            api_buffer[0] = '\x7E'; /* Start Delimiter */
//            api_buffer[3] = '\x10';    /* Frame Type = 0x10 = "Transmit Request" */
//            api_buffer[4] = '\x00';    /* Frame ID */
//            memcpy(api_buffer + 5, "\x00\x13\xA2\x00\x41\x93\x2D\xE3", 8);  /* 64-bit dest. address */
//            memcpy(api_buffer + 13, "\x00\x00", 2); /* 16-bit dest. address */
//            api_buffer[15] = '\x00';   /* Broadcast Radius */
//            api_buffer[16] = '\x00';   /* Options */
//            for(uint8_t i = 3; i < 17; i++) previous_checksum += api_buffer[i];
//
//            break;
//
//        case STRING_API:
//        break;
//
//        case BYTES_TRANSPARENT:
//        break;
//
//        case STRING_TRANSPARENT:
//        	break;
//    }
//}

//uint8_t xbeeApiModeSend(uint8_t *buff_message, uint16_t buff_size)
//{
//    /*Send chars through UART (Max 255 chars)*/
//
//    /*Aux variables*/
//    uint16_t i = 0;
//    uint16_t checksum = 0;
//    uint16_t length = 0;
//    uint8_t api_buffer[256];
//
//    /*If the buff size is larger than 255, return 'fail'*/
//    if (buff_size > 255)  return 0;
//    length = buff_size + 14; /* Number of bytes between length and checksum fields */
//
//    /* Creating the buffer to send with API Frame */
//    api_buffer[0] = '\x7E'; /* Start Delimiter */
//
//    api_buffer[1] = length >> 8;    /* Highest 8 bits of length */
//    api_buffer[2] = 0xFF & length;  /* Lowest 8 bits of length */
//
//    api_buffer[3] = '\x10';    /* Frame Type = 0x10 = "Transmit Request" */
//    api_buffer[4] = '\x01';    /* Frame ID */
//
//    memcpy(api_buffer + 5, "\x00\x13\xA2\x00\x41\x93\x2D\xE3", 8);  /* 64-bit dest. address */
//    memcpy(api_buffer + 13, "\x00\x00", 2); /* 16-bit dest. address */
//
//    api_buffer[15] = '\x00';   /* Broadcast Radius */
//    api_buffer[16] = '\x00';   /* Options */
//
//    memcpy(api_buffer + 17, buff_message, buff_size); /* RF Data */
//
//    /* Checksum calc */
//    checksum = previous_checksum;
//    for(i = 17; i < buff_size + 1; i++) checksum += api_buffer[i];
//
//
//    checksum = checksum & 0xFF; /* keep only the lowest 8 bits */
//    checksum = 0xFF - checksum;  /* Negate it and get checksum */
//    api_buffer[18 + buff_size] = checksum;
//
//    HAL_UART_Transmit (&huart2, api_buffer, length + 4, 1000); /*Send 'checksum'*/
//
//    return 1;  /*Return 'success'*/
//}

void xbeeSend(int id, ...) {
	va_list args;
	va_start(args, id);

	uint16_t data_word[4];

	for (int i = 0; i < 4; i++) {
		data_word[i] = (uint16_t) va_arg(args, int);
	}
	va_end(args);

	uint8_t xbeeBuffer[128];
	int len;
	switch (mode) {
	case BYTES_API:
		xbeeBuffer[0] = (id);
		xbeeBuffer[1] = (id >> 8);
		memcpy(xbeeBuffer + 2, data_word, 8);
		len = 10;
		xbeeApiModeSend(xbeeBuffer, len);
		break;

	case STRING_API:
		len = sprintf(xbeeBuffer, "%u\t%u\t%u\t%u\t%u", id, data_word[0],
				data_word[1], data_word[2], data_word[3]);
		xbeeApiModeSend(xbeeBuffer, len);
		break;

	case STRING_TRANSPARENT:
		len = sprintf(xbeeBuffer, "%u\t%u\t%u\t%u\t%u\n", id, data_word[0],
				data_word[1], data_word[2], data_word[3]);
		HAL_UART_Transmit(&huart2, xbeeBuffer, len, 100);
		break;

	case BYTES_TRANSPARENT:
		memcpy(xbeeBuffer, id, sizeof(uint16_t));
		memcpy(xbeeBuffer + 2, data_word, sizeof(uint16_t) * 4);
		memcpy(xbeeBuffer + 10, '\n', 1);
		HAL_UART_Transmit(&huart2, xbeeBuffer, 11, 100);
	}

	HAL_Delay(DELAY_XBEE);
}
