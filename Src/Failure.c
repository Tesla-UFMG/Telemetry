#include "ID.h"
#include "lcd_dash.h"
#include "Failure.h"
#include <string.h>

#define _ERRO_CONTROLE ECU_erro_bin_to_int()
#define _ERRO_INVERSOR INVERSOR_flag()
#define _ERRO_SEGURANCA BMS_erro_bin_to_int()

#define CONTROLE_flags_timer 2000
#define INVERSOR_flags_timer 2000
#define SEGURANCA_flags_timer 2000

uint16_t ERRO_INVERSOR;
uint16_t ERRO_CONTROLE;
uint16_t ERRO_SEGURANCA;

uint32_t flagsTimeout = 0;
int8_t FLAG_POP_UP = 0;
uint16_t ERRO = 0;
NextionError_e actual_erro = CONTROLE;

extern CanIdData_t can_vector[CAN_IDS_NUMBER];
extern int8_t FLAG_MSG;

uint8_t ECU_erro_bin_to_int() {
	for (int8_t i = 15; i >= 0; i--) {
		if ((can_vector[ID_control_torque_motor].word_0 >> i) == 1) {
			return i + 1;
		}
	}
	return 0;
}

uint8_t BMS_erro_bin_to_int() {
	for (int8_t i = 8; i >= 0; i--)
		if ((can_vector[ID_safety_bms].word_1) >> i == 1)
			return i + 1;
	return 0;
}

uint16_t INVERSOR_flag() {
	uint16_t FALHA = 0;
	uint16_t ALARME = 0;

	if (can_vector[ID_control_state_l_inv].word_1
			> can_vector[ID_control_state_r_inv].word_1)
		FALHA = can_vector[ID_control_state_l_inv].word_1;
	else
		FALHA = can_vector[ID_control_state_r_inv].word_1;

	if (can_vector[ID_control_state_l_inv].word_2
			> can_vector[ID_control_state_r_inv].word_2)
		ALARME = can_vector[ID_control_state_l_inv].word_2;
	else
		ALARME = can_vector[ID_control_state_r_inv].word_2;

	if (FALHA != 0)
		return FALHA;
	else if (ALARME != 0)
		return ALARME;
	return 0;
}

void FAILURE_IT() {
	/*Verify if have an control flag error during 1s*/
	ERRO_CONTROLE = _ERRO_CONTROLE;
	ERRO_INVERSOR = _ERRO_INVERSOR;
	ERRO_SEGURANCA = _ERRO_SEGURANCA;

	switch (actual_erro) {
	case CONTROLE:
		ERRO = _ERRO_CONTROLE;

		if (timer_wait_ms(flagsTimeout, CONTROLE_flags_timer))
			ERRO = 0;

		switch (ERRO) {
		case 1:
			FLAG_POP_UP = 1;
			return;
		case 2:
			FLAG_POP_UP = 2;
			return;
		case 8:
			FLAG_POP_UP = 46;
			return;
		case 9:
			FLAG_POP_UP = 47;
			return;
		case 10:
			FLAG_POP_UP = 48;
			return;
		default:
			actual_erro = INVERSOR;
			timer_restart(&flagsTimeout);
		}
	case INVERSOR:
		/*Verify if have an control flag status during 1s*/
		ERRO = _ERRO_INVERSOR;

		if (timer_wait_ms(flagsTimeout, CONTROLE_flags_timer))
			ERRO = 0;

		switch (ERRO) {
		case 46:
			FLAG_POP_UP = 3;
			return;
		case 47:
			FLAG_POP_UP = 4;
			return;
		case 50:
			FLAG_POP_UP = 5;
			return;
		case 90:
			FLAG_POP_UP = 6;
			return;
		case 98:
			FLAG_POP_UP = 7;
			return;
		case 110:
			FLAG_POP_UP = 8;
			return;
		case 128:
			FLAG_POP_UP = 9;
			return;
		case 133:
			FLAG_POP_UP = 10;
			return;
		case 134:
			FLAG_POP_UP = 11;
			return;
		case 135:
			FLAG_POP_UP = 12;
			return;
		case 136:
			FLAG_POP_UP = 13;
			return;
		case 152:
			FLAG_POP_UP = 14;
			return;
		case 702:
			FLAG_POP_UP = 15;
			return;
		case 704:
			FLAG_POP_UP = 16;
			return;
		case 706:
			FLAG_POP_UP = 17;
			return;
		case 21:
			FLAG_POP_UP = 18;
			return;
		case 22:
			FLAG_POP_UP = 19;
			return;
		case 30:
			FLAG_POP_UP = 20;
			return;
		case 34:
			FLAG_POP_UP = 21;
			return;
		case 38:
			FLAG_POP_UP = 22;
			return;
		case 48:
			FLAG_POP_UP = 23;
			return;
		case 51:
			FLAG_POP_UP = 24;
			return;
		case 67:
			FLAG_POP_UP = 25;
			return;
		case 71:
			FLAG_POP_UP = 26;
			return;
		case 72:
			FLAG_POP_UP = 27;
			return;
		case 78:
			FLAG_POP_UP = 28;
			return;
		case 79:
			FLAG_POP_UP = 29;
			return;
		case 80:
			FLAG_POP_UP = 30;
			return;
		case 84:
			FLAG_POP_UP = 31;
			return;
		case 91:
			FLAG_POP_UP = 32;
			return;
		case 99:
			FLAG_POP_UP = 33;
			return;
		case 150:
			FLAG_POP_UP = 34;
			return;
		case 153:
			FLAG_POP_UP = 35;
			return;
		case 156:
			FLAG_POP_UP = 36;
			return;
		case 157:
			FLAG_POP_UP = 37;
			return;
		case 158:
			FLAG_POP_UP = 38;
			return;
		case 182:
			FLAG_POP_UP = 39;
			return;
		case 183:
			FLAG_POP_UP = 40;
			return;
		case 185:
			FLAG_POP_UP = 41;
			return;
		case 228:
			FLAG_POP_UP = 42;
			return;
		case 234:
			FLAG_POP_UP = 43;
			return;
		case 235:
			FLAG_POP_UP = 44;
			return;
		case 711:
			FLAG_POP_UP = 45;
			return;
		default:
			actual_erro = SEGURANCA;
			timer_restart(&flagsTimeout);
		}
	case SEGURANCA:
		/*Verify if have an security flag error during 1s after control*/
		ERRO = _ERRO_SEGURANCA;

		if (timer_wait_ms(flagsTimeout, CONTROLE_flags_timer))
			ERRO = 0;

		switch (ERRO) {
		case 1:
			FLAG_POP_UP = 49;
			return;
		case 2:
			FLAG_POP_UP = 50;
			return;
		case 3:
			FLAG_POP_UP = 51;
			return;
		default:
			actual_erro = DEFAULT;
			timer_restart(&flagsTimeout);
		}
	case DEFAULT:
		/*If don't have error, flags error return to 0 */
		if (_ERRO_CONTROLE == 0 && _ERRO_INVERSOR == 0 && _ERRO_SEGURANCA == 0)
			FLAG_POP_UP = 0;

		/*If don't have error, displays message flags*/
		if (FLAG_POP_UP == 0)
			FLAG_POP_UP = -FLAG_MSG; //massages are negative because of the organization of IDS in Nextion

		actual_erro = CONTROLE;
		timer_restart(&flagsTimeout);
	}
}
