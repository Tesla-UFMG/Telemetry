#include "telemetry.h"


extern CanIdData_t can_vector[CAN_IDS_NUMBER];
extern uint8_t uart_user_message[DMA_RX_BUFFER_SIZE];	/* Buffer received for user access */
extern UART_HandleTypeDef huart2;
extern Timer_t packTimer;
extern uint32_t actualTimer;
extern SendMode_e mode;
extern uint8_t FLAG_ERRO;
uint16_t api_length = 0;

/* Telemetry variables */
uint8_t _real_clock_received = 0;


uint8_t compareString(uint8_t *first, uint8_t *second, uint16_t len)
{
  while (*first == *second) {

    len--;
    if(len == 0)  return 1; /* Same strings */
     
    first++;
    second++;
  }

  return 0;
}

void uart2MessageReceived(void)
{
  uint16_t checksum = 0;
  uint8_t return_status = 0;
  const uint8_t* ping_request = "ping";


  if(uart_user_message[3] != 0x90)  return; /* If the message received != "Receive Packet" */

  return_status = compareString(uart_user_message + 15, ping_request, 4); /* Comparing if the received message is the xbee ping request */
  if(return_status) return;


  switch (mode)
  {
  case BYTES_API:
	blinkLed3();
    api_length = uart_user_message[1] << 8;
    api_length += uart_user_message[2];
    if(api_length > 255)  return; /* If api message length > DMA_RX_BUFFER_SIZE */

    FLAG_ERRO = uart_user_message[15];


    break;

  default:
    break;
  }
}
//
//void realClockRequest(void)
//{
//  uint8_t can_vet_tx[8];
//
//  /* 1) Global variables Init */
//  can_vector[10].word_0 = 1;
//  can_vector[10].word_1 = 0;
//  can_vector[10].word_2 = 0;
//  can_vector[10].word_3 = 0;
//
//  /* 2) Sending clock request */
//  for(uint8_t i = 0; i < 41; i++){
//
//    blinkLed1();
//    blinkLed2();
//    blinkLed3();
//    HAL_Delay(50);
//
//    if(_real_clock_received == 1){
//     UART_Print_Debug("_realclock received, por isso to saindo da requisição\n\r");
//     UART_Print_Debug("Número de tentativas = %u\n\r", i);
//    }
//    else{
//      xbeeSend(10, BYTES_API, can_vector[REAL_CLK_CAN_ID].word_0, can_vector[10].word_1, can_vector[10].word_2, can_vector[10].word_3);
//    }
//  }
//
//  /* Sending the real clock in can bus */
//  can_vet_tx[0] = can_vector[REAL_CLK_CAN_ID].word_0;
//  can_vet_tx[1] = can_vector[REAL_CLK_CAN_ID].word_0 >> 8;
//  can_vet_tx[2] = can_vector[REAL_CLK_CAN_ID].word_1;
//  can_vet_tx[3] = can_vector[REAL_CLK_CAN_ID].word_1 >> 8;
//  can_vet_tx[4] = can_vector[REAL_CLK_CAN_ID].word_2;
//  can_vet_tx[5] = can_vector[REAL_CLK_CAN_ID].word_2 >> 8;
//  can_vet_tx[6] = can_vector[REAL_CLK_CAN_ID].word_3;
//  can_vet_tx[7] = can_vector[REAL_CLK_CAN_ID].word_3 >> 8;
//  CAN_Transmit(can_vet_tx, REAL_CLK_CAN_ID);
//}

void xbeePacks(void)
{
  uint16_t i;

  /*Pack 0*/
  for(i = 260; i < 264; i++)
    xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1, can_vector[i].word_2, can_vector[i].word_3);

  /*Pack 1*/
  for(i = 265; i < 269; i++)
    xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1, can_vector[i].word_2, can_vector[i].word_3);

  /*Pack 2*/
  for(i = 270; i < 274; i++)
    xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1, can_vector[i].word_2, can_vector[i].word_3);

  /*Pack 3*/
  for(i = 275; i < 279; i++)
    xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1, can_vector[i].word_2, can_vector[i].word_3);

  /*Pack 4*/
  for(i = 280; i < 284; i++)
    xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1, can_vector[i].word_2, can_vector[i].word_3);

  /*Pack 5*/
  for(i = 285; i < 289; i++)
    xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1, can_vector[i].word_2, can_vector[i].word_3);
}

void xbeeGeneral(void)
{
  uint16_t i;

	/*Bateria General Information*/
	xbeeSend(0, can_vector[0].word_0, 0, 0, 0);
  for(i = 51; i < 55; i++)
    xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1, can_vector[i].word_2, can_vector[i].word_3);



	/*Aquisição General Information*/
  xbeeSend(170, can_vector[170].word_0, can_vector[170].word_1, can_vector[170].word_2, can_vector[170].word_3);
  xbeeSend(171, can_vector[171].word_0, can_vector[171].word_1, can_vector[171].word_2, can_vector[171].word_3);
  xbeeSend(161, can_vector[161].word_0, can_vector[161].word_1, can_vector[161].word_2, can_vector[161].word_3);
  xbeeSend(162, can_vector[161].word_0, can_vector[161].word_1, can_vector[161].word_2, can_vector[161].word_3);
  for(i = 156; i < 160; i++)
    xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1, can_vector[i].word_2, can_vector[i].word_3);


  
	/*Controle Information*/
  xbeeSend(1, can_vector[1].word_0, can_vector[1].word_1, can_vector[1].word_2, can_vector[1].word_3);
  for(i = 101; i < 108; i++)
    xbeeSend(i, can_vector[i].word_0, can_vector[i].word_1, can_vector[i].word_2, can_vector[i].word_3);
}

void telemetrySend(void)
{
  if((actualTimer-packTimer.previous)>packTimer.interval){
    xbeePacks();
    packTimer.previous = HAL_GetTick();
  }
  xbeeGeneral();
}
