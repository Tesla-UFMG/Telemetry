/*
 * telemetry.h
 *
 *  Created on: 30 de sept de 2019
 *      Author: Formula Tesla UFMG
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include <Interface_Master.h>

void uart2MessageReceived(void);
uint8_t compareString(uint8_t *first, uint8_t *second, uint16_t len);
void uart2MessageReceived(void);
void realClockRequest(void);
void telemetrySend(void);



#endif /* TELEMETRY_H_ */
