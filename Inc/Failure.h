/*
 * Failure.h
 *
 *  Created on: Jul 14, 2023
 *      Author: Gabriel
 */

#ifndef FAILURE_H_
#define FAILURE_H_

#include <Interface_Master.h>

uint8_t ECU_erro_bin_to_int();
uint16_t INVERSOR_flag();
uint8_t BMS_erro_bin_to_int();
void FAILURE_IT();


#endif /* FAILURE_H_ */
