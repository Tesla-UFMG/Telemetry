#include "lcd_dash.h"
#include "id.h"

/*Defines*/
#define bool uint8_t
#define DISPLAY_CURRENT_PAGE_COMMAND 0x66
#define BOTAO_STATE HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)

/*Extern variables*/
extern CanIdData_t can_vector[CAN_IDS_NUMBER];
extern uint8_t uart_user_message[DMA_RX_BUFFER_SIZE]; /* Buffer received for user access */

/* Nextion Variables */
NextionPage_e actual_page = PAGE0;
uint8_t pageMessageReceived;
uint32_t pageTimeout;
static uint32_t updateTimer;

/* Variables to nextion test loop */
uint8_t FLAG_ERRO = 0;
uint8_t PAGE = 0;
uint8_t last_state = 0;
uint8_t botao = 0;
uint8_t CAN_STATE = 0;

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
	/* If the message is to change the nextion page */
	if (uart_user_message[0] == DISPLAY_CURRENT_PAGE_COMMAND) {
		pageMessageReceived = 1;
		actual_page = (NextionPage_e) uart_user_message[1];
	}
}
void nextion_init_can() {
	pageMessageReceived = 0;
	timer_restart(&pageTimeout);

	while (!pageMessageReceived) {
		sendCommand("sendme");
		if (timer_wait_ms(pageTimeout, 300)) {
			pageMessageReceived = 0;
		}
	}

	CAN_STATE = 0;
	PAGE = actual_page;
}
void nextionLoop(void) {
	if (timer_wait_ms(updateTimer, 50)) {

		pageMessageReceived = 0;
		timer_restart(&pageTimeout);
		sendCommand("sendme");

		while (!pageMessageReceived) {
			if (timer_wait_ms(pageTimeout, 50)) {
				pageMessageReceived = 0;
				return;
			}
		}

		if (actual_page != PAGE) {
			NexPageShow(PAGE);
		}

		if (BOTAO_STATE == 0 && last_state == 0) {
			last_state++;
			if (PAGE == PAGE3)
				PAGE = PAGE0;
			else
				PAGE++;
		} else if (BOTAO_STATE != 0 && last_state != 0)
			last_state = 0;

		if (can_vector[1].word_2 == 0 && botao == 0) {
			botao++;
			if (FLAG_ERRO == 5)
				FLAG_ERRO = 0;
			else
				FLAG_ERRO++;
		} else if (can_vector[1].word_2 != 0 && botao != 0)
			botao = 0;

		switch (actual_page) {
		case PAGE0:

			break;

		case PAGE1:
            if (CAN_STATE)
            {
			NexNumberSetValue(0, 66);
			NexNumberSetValue(1, 155);
			NexNumberSetValue(2, can_vector[1].word_1);
			NexXfloatSetValue(0, 1648);
			NexXfloatSetValue(1, 265);
			NexXfloatSetValue(2, 418);
			NexTextSetText(0, "Modo turbo");
			NexTextSetText(1, "3/4");
			NexPictureSetPic(1, 50 + FLAG_ERRO);
            }
			NexVariableSetValue(1, !CAN_STATE);
			break;

		case PAGE2:

			break;

		case PAGE3:

			break;

		case PAGE4:

			break;
		}
	}
	timer_restart(&updateTimer);
}
