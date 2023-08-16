#include <DMA_USART.h>
#include <ID.h>
#include <lcd_dash.h>
#include <nextion.h>
#include <stm32f103xb.h>
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_can.h>
#include <stm32f1xx_hal_gpio.h>
#include <string.h>
#include <sys/_stdint.h>

/*Defines*/
#define DISPLAY_CURRENT_PAGE_COMMAND 0x66
#define BOTAO_STATE HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)

/*Extern variables*/
extern CAN_HandleTypeDef hcan;
extern CanIdData_t can_vector[CAN_IDS_NUMBER];

extern uint8_t DMA_RX_Buffer_3[DMA_RX_BUFFER_SIZE];

extern int8_t FLAG_POP_UP;
extern int8_t FLAG_MSG;

extern void FAILURE_IT();

/* Nextion Variables */

NextionPage_e actual_page = PAGE0;
NextionPage_e previus_page = PAGE1;
NextionPage_e PAGE = 0;
uint8_t pageMessageReceived = 0;
uint8_t NEXTION_UART_BUFFER[DMA_RX_BUFFER_SIZE_NEXTION]; /* Buffer received for user access */

/* Timer Variables */

uint32_t updateTimer;
uint32_t timer_actual_nextion;
uint32_t pageTimeout = 0;
uint32_t botaoTimeout = 0;
uint32_t modoTimeout = 0;

/* LCD Page Variables */
#define VELOCIDADE RPM_To_KMH_speed()
#define TORQUE can_vector[ID_control_hodometer].word_1
#define CARGA can_vector[ID_safety_charge].word_0
#define TENSAO can_vector[ID_safety_voltage].word_1
#define HODOM can_vector[ID_control_hodometer].word_3
#define TEMPERATURA can_vector[ID_safety_voltage].word_3
#define MODO_FLAG can_vector[ID_control_hodometer].word_0
#define AIR_FLAG can_vector[ID_safety_bms].word_2

char MODO[11];
char AIR[11];
char BRAKE[5] = "3/4";

/* Variables to nextion test loop */
uint8_t PAGE_ERRO = 0;
uint8_t previus_MODO_FLAG = 0;
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

uint8_t RPM_To_KMH_speed() {
	float speed = (can_vector[ID_control_speed_l_motor].word_0
			+ can_vector[ID_control_speed_r_motor].word_0) / 8.89;
	speed = speed / 2;
	return 0.1885 * speed * 0.52;
}

void USART3_Message_Received(void) {

	/*Verify if the message is to change the nextion page */
	memcpy(NEXTION_UART_BUFFER, DMA_RX_Buffer_3, DMA_RX_BUFFER_SIZE_NEXTION);

	if (NEXTION_UART_BUFFER[0] == DISPLAY_CURRENT_PAGE_COMMAND) {
		timer_actual_nextion = HAL_GetTick();
		pageMessageReceived = 1;
		actual_page = (NextionPage_e) NEXTION_UART_BUFFER[1];
	}

//	for (uint8_t i = 0; i < DMA_RX_BUFFER_SIZE_NEXTION; i++)
//		NEXTION_UART_BUFFER[i] = 0;
}

void NEXTION_BusOff_Verify() {
	if (HAL_GetTick() - timer_actual_nextion > 2000)
		HAL_CAN_Stop(&hcan);
}

void NEXTION_Init() {
	pageMessageReceived = 0;
	sendCommand("sendme");

	if (CAN_STATE == 0)
		NexVariableSetValue(1, !CAN_STATE);

	timer_restart(&pageTimeout);

	/*Recept the page value and reset state of can*/
	while (!pageMessageReceived) {
		NEXTION_BusOff_Verify();
		if (timer_wait_ms(pageTimeout, 50)) {
			sendCommand("sendme");
			pageMessageReceived = 0;
			timer_restart(&pageTimeout);
		}
	}

	if (actual_page != PAGE0)
		if (actual_page != PAGE1)
			NexPageShow(1);

	PAGE = actual_page;
	previus_page = PAGE;
}

void NEXTION(void) {

	NEXTION_BusOff_Verify();

	if (timer_wait_ms(updateTimer, 0)) {

		if (can_vector[ID_control_torque_motor].word_1 == 0) {
			if (MODO_IT() == 1)
				return;
		} else
			PAGE = PAGE1;

		FAILURE_IT();

		if (can_vector[ID_control_torque_motor].word_1 == 0) {
			AIR_IT();
//			BOTAO_IT();
		}

		PAGE_IT();

		switch (actual_page) {
		case PAGE0:

			break;

		case PAGE1:
			if (CAN_STATE) {
				NexPictureSetPic(1, 54 + FLAG_POP_UP);
				NexNumberSetValue(0, VELOCIDADE);
				NexNumberSetValue(1, TORQUE*10);
				NexNumberSetValue(2, CARGA);
				NexXfloatSetValue(0, TENSAO/100);
				NexXfloatSetValue(1, HODOM/10);
				NexXfloatSetValue(2, TEMPERATURA);
				NexTextSetText(0, MODO);
				NexTextSetText(1, BRAKE);
			}
			NexVariableSetValue(1, !CAN_STATE);
			break;

		case PAGE2:
			NexXfloatSetValue(3, HODOM);
			NexXfloatSetValue(4, TEMPERATURA);
			NexTextSetText(0, MODO);
			NexTextSetText(3, AIR);
			if (AIR_FLAG != 1)
			sendCommand("t3.pco=63488");
			else
			sendCommand("t3.pco=1732");
			break;

			case PAGE3:

			break;

			case PAGE4:

			break;

			default:

			break;
		}
		timer_restart(&updateTimer);
	}
}

uint8_t MODO_IT() {
	switch (MODO_FLAG) {
		case 1:
		strcpy(MODO, "Enduro");
		break;
		case 2:
		strcpy(MODO, "Aceleracao");
		break;
		case 3:
		strcpy(MODO, "Skidpad");
		break;
		case 4:
		strcpy(MODO, "Autocross");
		break;
		default:
		strcpy(MODO, "FAILURE");
		break;

	}
	if (previus_MODO_FLAG != MODO_FLAG && previus_MODO_FLAG != 0)
	{
		if (previus_page == PAGE) {
			timer_restart(&modoTimeout);
			previus_page = PAGE;}
		PAGE = PAGE4;
		PAGE_IT();
		NexPictureSetPic(0, 106 + MODO_FLAG);
		if(timer_wait_ms(modoTimeout, 1500))
		{
			previus_MODO_FLAG = MODO_FLAG;
			PAGE = previus_page;
			PAGE_IT();
		}
		return 1;
	}
	return 0;
}

void AIR_IT() {
	switch (AIR_FLAG) {
		case 0:
		strcpy(AIR, "FECHADO");
		break;
		case 1:
		strcpy(AIR, "ABERTO");
		break;
		default:
		strcpy(AIR, "FALHA");
		break;
	}
}

/*Get and modify actual page*/
void PAGE_IT() {
	pageMessageReceived = 0;
	timer_restart(&pageTimeout); //initializes a timer to count response time
	sendCommand("sendme"); // send command to get the page

	while (!pageMessageReceived) {
		if (timer_wait_ms(pageTimeout, 50)) { //if the page does not match, it will send a command to switch the page
			pageMessageReceived = 0;
			return;
		}
	}
	if (previus_page > PAGE2)
		previus_page = PAGE1;
	if (actual_page != PAGE) { //if the page does not match, it will send a command to switch the page
		NexPageShow(PAGE);
	}
}

/*Get state of bottom to replace page*/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
				if (FLAG_MSG != 0) {
					FLAG_MSG = 0;
					return;
				}

				if (PAGE == PAGE2)
					PAGE = PAGE1;
				else
					PAGE++;
}

//void BOTAO_IT() {
//
//	if (BOTAO_STATE == 0 && last_state == 0) {
//		if (timer_wait_ms(botaoTimeout, 100)) {
//			last_state++;
//			if (FLAG_MSG != 0) {
//				FLAG_MSG = 0;
//				return;
//			}
//
//			if (PAGE == PAGE2)
//				PAGE = PAGE1;
//			else
//				PAGE++;
//		}
//	} else if (BOTAO_STATE != 0 && last_state != 0) {
//		timer_restart(&botaoTimeout);
//		last_state = 0;
//	}
//}
