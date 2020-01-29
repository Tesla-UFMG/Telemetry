/*Criado por Francisco Gomes - Kotchergenko Engenharia 2018*/

#ifndef NRF24_H_
#define NRF24_H_

#include "stm32f1xx_hal.h"
#include "string.h"
#include "stdbool.h"

/*Commands*/
#define R_REGISTER 0x00
#define W_REGISTER 0x20
#define R_RX_PAYLOAD 0x61
#define W_TX_PAYLOAD 0xA0
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define REUSE_TX_PL 0xE3
#define R_RX_PL_WID 0x60
#define W_ACK_PAYLOAD 0xA8
#define W_TX_PAYLOAD_NO_ACK 0xB0
#define NOP 0xFF

/*Memory Map*/
//Configuration Register
#define CONFIG 0x00
#define MASK_RX_DR 6
#define MASK_TX_DS 5
#define MASK_MAX_RT 4
#define EN_CRC 3
#define CRCO 2
#define PWR_UP 1
#define PRIM_RX 0

//Enhanced ShockBurst
#define EN_AA 0x01
#define ENAA_P5 5
#define ENAA_P4 4
#define ENAA_P3 3
#define ENAA_P2 2
#define ENAA_P1 1
#define ENAA_P0 0

//Enabled RX Address
#define EN_RXADDR 0x02
#define ERX_P5 5
#define ERX_P4 4
#define ERX_P3 3
#define ERX_P2 2
#define ERX_P1 1
#define ERX_P0 0

//Setup Address Widths
#define SETUP_AW 0x03
#define AW 0

//Setup Automatic Retransmission
#define SETUP_RETR 0x04
#define ARD 4
#define ARC 0

//RF Channel
#define RF_CH 0x05
#define RF_CH_B 0

//RF Setup Register
#define RF_SETUP 0x06
#define CONT_WAVE 7
#define RF_DR_LOW 5
#define PLL_LOCK 4
#define RF_DR_HIGH 3
#define RF_PWR 1

//Status Register
#define STATUS 0x07
#define RX_DR 6
#define TX_DS 5
#define MAX_RT 4
#define RX_P_NO 1
#define TX_P_FULL 0

//Transmit Observe Register
#define OBSERVE_TX 0x08
#define PLOS_CNT 4
#define ARC_CNT 0

//Received Power Detector
#define RPD 0x09
#define RPD_B 0

//Receive Addresses Data Pipes
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F

//Transmit Address
#define TX_ADDR 0x10

//Widths of RX Pyloads
#define RX_PW_P0 0x11
#define RX_PW_P0_B 0
#define RX_PW_P1 0x12
#define RX_PW_P1_B 0
#define RX_PW_P2 0x13
#define RX_PW_P2_B 0
#define RX_PW_P3 0x14
#define RX_PW_P3_B 0
#define RX_PW_P4 0x15
#define RX_PW_P4_B 0
#define RX_PW_P5 0x16
#define RX_PW_P5_B 0

//FIFO Status Register
#define FIFO_STATUS 0x17
#define TX_REUSE 6
#define TX_FULL 5
#define TX_EMPTY 4
#define RX_FULL 1
#define RX_EMPTY 0

//Enable Dynamic Payload Length
#define DYNPD 0x1C
#define DPL_P5 5
#define DPL_P4 4
#define DPL_P3 3
#define DPL_P2 2
#define DPL_P1 1
#define DPL_P0 0

//Feature Register
#define FEATURE 0x1D
#define EN_DPL 2
#define EN_ACK_PAY 1
#define EN_DYN_ACK 0

/*End of Memory Map*/
/**/

/*Defines*/
#define _250kbps 2
#define _1Mbps 0
#define _2Mbps 1

#define _18dBm 0
#define _12dBm 1
#define _6dBm 2
#define _0dBm 3

#define _250us 0
#define _500us 1
#define _750us 2
#define _1000us 3
#define _1250us 4
#define _1500us 5
#define _1750us 6
#define _2000us 7
#define _2250us 8
#define _2500us 9
#define _2750us 10
#define _3000us 11
#define _3250us 12
#define _3500us 13
#define _3750us 14
#define _4000us 15

#define noRetries 0
#define _1retrie 1
#define _2retries 2
#define _3retries 3
#define _4retries 4
#define _5retries 5
#define _6retries 6
#define _7retries 7
#define _8retries 8
#define _9retries 9
#define _10retries 10
#define _11retries 11
#define _12retries 12
#define _13retries 13
#define _14retries 14
#define _15retries 15

/*Declara��eses da biblioteca (*.h) */

void RF24init(SPI_HandleTypeDef *HSPI, int ce, int csn, uint8_t channel, unsigned int baudrateOverTheAir, unsigned int transmitPower);

uint8_t RF24send(char* _data, unsigned int _size, unsigned int _pipe);
void RF24sendInTheNextReceive(char* _data, unsigned int _size, unsigned int _pipe);
void RF24Flush();

void RF24attrAddress2Pipe0(unsigned int* address);
void RF24attrAddress2Pipe1(unsigned int* address);
void RF24attrAddress2Pipe2(unsigned int address);
void RF24attrAddress2Pipe3(unsigned int address);
void RF24attrAddress2Pipe4(unsigned int address);
void RF24attrAddress2Pipe5(unsigned int address);

unsigned int RF24available();
uint8_t RF24checkRetries();

void RF24read(char* _data);
void RF24enablePipe(unsigned int _pipe);
void RF24disablePipe(unsigned int _pipe);

unsigned int readRegister(unsigned int reg);
void readRegisterManyTimes(unsigned int reg, unsigned int* _data, int nBytes);
void RF24SetRetries(uint8_t retr, uint16_t delay);

#endif /* NRF24_H_ */
