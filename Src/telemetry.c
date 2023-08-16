#include "telemetry.h"
#include "ID.h"

extern CanIdData_t can_vector[CAN_IDS_NUMBER];
extern UART_HandleTypeDef huart2;
extern uint32_t actualTimer;
extern SendMode_e mode;
extern uint8_t ECU_erro_bin_to_int();
extern uint8_t BMS_erro_bin_to_int();
extern uint8_t DMA_RX_Buffer_2[DMA_RX_BUFFER_SIZE];

#define _ERRO_CONTROLE ECU_erro_bin_to_int()
#define _ERRO_INVERSOR can_vector[ID_control_torque_motor].word_1
#define _ERRO_SEGURANCA BMS_erro_bin_to_int()
#define PACKS_Timer 2500

TelemetryINFO_e PACK = PACKS1;
uint32_t informacaoTimeout = 0;
uint8_t XBEE_UART_BUFFER[DMA_RX_BUFFER_SIZE_XBEE]; /* Buffer received for user access */
TelemetryINFO_e INFORMACAO = GERAL;
uint16_t api_length = 0;
int8_t FLAG_MSG = 0;

/* Telemetry variables */
uint8_t _real_clock_received = 0;

uint8_t compareString(uint8_t *first, uint8_t *second, uint16_t len) {
	while (*first == *second) {

		len--;
		if (len == 0)
			return 1; /* Same strings */

		first++;
		second++;
	}

	return 0;
}

void uart2MessageReceived(void) {
	uint8_t return_status = 0;
	uint8_t *ping_request = (uint8_t*) "ping";

	/* Data receive format

	 Start: 0x7E
	 Length: 0x?? 0x??
	 Flame type: 0x90
	 64 bit adress: 0x?? 0x?? 0x?? 0x?? 0x?? 0x?? 0x?? 0x??
	 16 bit adress: 0x?? 0x??
	 Receive options: 0x40
	 Receive data: 0x?? 0x?? 0x?? 0x?? ... ... maximum 255 bytes
	 Checksum: 0x??
	 */

	memcpy(XBEE_UART_BUFFER, DMA_RX_Buffer_2, DMA_RX_BUFFER_SIZE_XBEE);

	if (XBEE_UART_BUFFER[3] != 0x90)
		return; /* If the message received != "Receive Packet" */

	return_status = compareString(XBEE_UART_BUFFER + 15, ping_request, 4); /* Comparing if the received message is the xbee ping request */
	if (return_status)
		return;
	if (mode == BYTES_API) {
		blinkLed3();
		api_length = XBEE_UART_BUFFER[1] << 8;
		api_length += XBEE_UART_BUFFER[2];
		if (api_length > 255)
			return; /* If api message length > DMA_RX_BUFFER_SIZE */

		FLAG_MSG = XBEE_UART_BUFFER[15];
	}
//	for (uint8_t i = 0; i < DMA_RX_BUFFER_SIZE_NEXTION; i++)
//		XBEE_UART_BUFFER[i] = 0;
}

void xbeePACKS_1(void) {
	/*Pack 1*/
	for (int i = ID_safety_pack1_1; i <= ID_safety_pack1_5; i++)
		xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1,
				can_vector[i].word_2, can_vector[i].word_3);
}

void xbeePACKS_2(void) {
	/*Pack 2*/
	for (int i = ID_safety_pack2_1; i <= ID_safety_pack2_5; i++)
		xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1,
				can_vector[i].word_2, can_vector[i].word_3);
}

void xbeePACKS_3(void) {
	/*Pack 3*/
	for (int i = ID_safety_pack3_1; i <= ID_safety_pack3_5; i++)
		xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1,
				can_vector[i].word_2, can_vector[i].word_3);
}

void xbeePACKS_4(void) {
	/*Pack 4*/
	for (int i = ID_safety_pack4_1; i <= ID_safety_pack4_5; i++)
		xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1,
				can_vector[i].word_2, can_vector[i].word_3);
}

void xbeeGERAL(void) {
	/*GERAL*/
	xbeeSend(ID_safety_voltage, can_vector[ID_safety_voltage].word_0,
			can_vector[ID_safety_voltage].word_1, 0,
			can_vector[ID_safety_voltage].word_3);

	xbeeSend(ID_safety_bms, 0, _ERRO_SEGURANCA,
			can_vector[ID_safety_bms].word_2, can_vector[ID_safety_bms].word_3);

	xbeeSend(ID_safety_current, 0, can_vector[ID_safety_current].word_1, 0,
			can_vector[ID_safety_current].word_3);

	xbeeSend(ID_safety_charge, can_vector[ID_safety_charge].word_0, 0, 0,
			can_vector[ID_safety_charge].word_3);

	xbeeSend(ID_safety_soc, can_vector[ID_safety_soc].word_0, 0, 0, 0);

	xbeeSend(ID_control_speed_l_motor,
			can_vector[ID_control_speed_l_motor].word_0, 0, 0, 0);

	xbeeSend(ID_control_speed_r_motor,
			can_vector[ID_control_speed_r_motor].word_0, 0, 0, 0);
}

void xbeeCONTROLE(void) {
	/*CONTROLE*/
	xbeeSend(ID_control_accelerometer,
			can_vector[ID_control_accelerometer].word_0,
			can_vector[ID_control_accelerometer].word_1,
			can_vector[ID_control_accelerometer].word_2, 0);

	xbeeSend(ID_control_speed_average, 0,
			can_vector[ID_control_speed_average].word_1,
			can_vector[ID_control_speed_average].word_2,
			can_vector[ID_control_speed_average].word_3);

	xbeeSend(ID_control_hodometer, 0, 0, 0,
			can_vector[ID_control_hodometer].word_3);

	xbeeSend(ID_control_torque_motor, _ERRO_CONTROLE, _ERRO_INVERSOR, 0, 0);

	xbeeSend(ID_control_speed_wheel, can_vector[ID_control_speed_wheel].word_0,
			can_vector[ID_control_speed_wheel].word_1,
			can_vector[ID_control_speed_wheel].word_2,
			can_vector[ID_control_speed_wheel].word_3);
}

void TELEMETRY() {
	switch (INFORMACAO) {
	case GERAL:
		xbeeGERAL();
		INFORMACAO = CONTROL;
		break;

	case CONTROL:
		xbeeCONTROLE();
		INFORMACAO = PACKS;
		break;

	default:
		if (timer_wait_ms(informacaoTimeout, PACKS_Timer))
			switch (PACK) {
			case PACKS1:
				xbeePACKS_1();
				PACK = PACKS2;
				timer_restart(&informacaoTimeout);
				break;
			case PACKS2:
				xbeePACKS_2();
				PACK = PACKS3;
				timer_restart(&informacaoTimeout);
				break;
			case PACKS3:
				xbeePACKS_3();
				PACK = PACKS4;
				timer_restart(&informacaoTimeout);
				break;
			case PACKS4:
				xbeePACKS_4();
				PACK = PACKS1;
				timer_restart(&informacaoTimeout);
			}
		INFORMACAO = GERAL;
		break;
	}
}
