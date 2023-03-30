#include "lcd_dash.h"
#include "id.h"

/*Defines*/
#define bool uint8_t
#define true 1
#define false 0
#define DISPLAY_CURRENT_PAGE_COMMAND 0x66

/*Extern variables*/
extern CanIdData_t can_vector[CAN_IDS_NUMBER];
extern uint8_t uart_user_message[DMA_RX_BUFFER_SIZE]; /* Buffer received for user access */

/* Nextion Variables */
NextionPage_e actual_page = PAGE0;
NextionPage_e previous_page = PAGE0;
NextionAdvice_e actual_advice = NO_ADVICE;
uint8_t _flag_information_to_send = 0;
volatile static bool pageMessageReceived;
static uint32_t pageTimeout;
static uint32_t updateTimer;
uint32_t actual_timer;

/* Variables to nextion test loop */
uint8_t flag = 0;
uint32_t actual_timer;
uint32_t previous_timer;
uint8_t aux = 0;
uint8_t botao = 0;
uint8_t botao1 = 0;

/* Pages and informations:
 Page 0: GIF Init page
 Page 1: DRIVE
 Page 2: SYSTEMS
 Page 3: DEBUG
 Page 4: Alert area
 */

/* Dash Functions: */

void uart3MessageReceived(void) {

	blinkLed3();
	timerAtualization();
	/* If the message is to change the nextion page */
	if (uart_user_message[0] == DISPLAY_CURRENT_PAGE_COMMAND) {
		pageMessageReceived = 1;
		actual_page = (NextionPage_e) uart_user_message[1];
	}
}

void nextionLoop(void) {

	if (actual_page != aux) {
		actual_page = aux;
		NexPageShow(actual_page);
	}

	if (can_vector[1].word_3 == 0 && botao == 0) {
		botao++;
		if (actual_page == PAGE3)
			aux = PAGE0;
		else
			aux++;
	} else if (can_vector[1].word_3 != 0 && botao != 0)
		botao = 0;

	if (can_vector[1].word_2 == 0 && botao1 == 0) {
			botao1++;
			if (flag == 5)
				flag = 0;
			else
				flag++;
		} else if (can_vector[1].word_2 != 0 && botao1 != 0)
			botao1 = 0;

	switch (actual_page) {
	case PAGE0:

		break;

	case PAGE1:

		NexNumberSetValue(0, 66);
		NexNumberSetValue(1, 155);
		NexNumberSetValue(2, can_vector[1].word_1);
		NexXfloatSetValue(0, 1648);
		NexXfloatSetValue(1, 265);
		NexXfloatSetValue(2, 418);
		NexTextSetText(0, "Aceleracao");
		NexTextSetText(1, "3/4");
        NexPictureSetPic(1, 43 +  flag);
		break;

	case PAGE2:

		break;

	case PAGE3:

		break;

	case PAGE4:

		break;
	}
}

void nextionTestLoop(void) {
	return;
}

uint8_t timer_wait_ms(uint32_t timer_start, uint32_t delay) {
	const uint32_t current_time = HAL_GetTick();
	if ((current_time - timer_start) >= delay) {
		return true;
	}
	return false;
}
