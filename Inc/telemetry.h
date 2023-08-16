/*
 * telemetry.h
 *
 *  Created on: 30 de sept de 2019
 *      Author: Formula Tesla UFMG
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include <Interface_Master.h>

typedef enum {
	GERAL, CONTROL, PACKS ,PACKS1, PACKS2, PACKS3, PACKS4
} TelemetryINFO_e;

void uart2MessageReceived(void);
uint8_t compareString(uint8_t *first, uint8_t *second, uint16_t len);
void uart2MessageReceived(void);
void realClockRequest(void);
void TELEMETRY(void);



#endif /* TELEMETRY_H_ */
