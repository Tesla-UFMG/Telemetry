#include "lcd_dash.h"

/*Extern variables*/
extern CanIdData_t can_vector[CAN_IDS_NUMBER];
extern uint8_t uart_user_message[DMA_RX_BUFFER_SIZE];	/* Buffer received for user access */

/* Nextion Variables */
NextionPage_e actual_page = PAGE0;
NextionPage_e previous_page = PAGE0;
NextionAdvice_e actual_advice = NO_ADVICE;
uint8_t _flag_information_to_send = 0;

/* Variables to nextion test loop */
uint8_t auxiliar;
uint32_t actual_timer;
uint32_t previous_timer;
uint8_t aux;



/* Pages and informations:

Page 0: GIF Init page

Page 1: Drive Page:
-Battery Percent(j0, n0)
-Hodometer(x1)
-Speed(x0)
-B.Bias(n3)
-TC(t0)
-Torque(n1)

Page 2: Enduro Page:
-Motor Efficiency(p1.pic = 9 = Good Efficiency; p1.pic = 10 = Bad Efficiency)
-Battery Percent(j1, n4)
-Hodometer(x0)
-Regen brake bar(j0 -> 34 < 67 < 100)
-Speed(x1)
-B.Bias(n3)
-TC(t0)
-Torque(n0)
-Distance Per Charge(p0.pic -> 5 > 38 > 39 > 6)

Page 3: Eletronic Systems
-Max temperature(x0)
-Min Voltage(x1)
-Total Voltage(x2)
-Mode(t0)
-Status ECU(t1)
-GLV Voltage(x3)
-Total Hodometer(x4)
-Status Datalog(t2)
-Rocker(n2)
-Hour(n0)
-Minute(n1)

Page 4: Debug Area
-Id 350_0: n0
-Id 350_1: n5
-Id 350_2: n6
-Id 351_0: n1
-Id 351_1: n4
-Id 351_2: n7
-Id 352_0: n2
-Id 352_1: n3
-Id 352_2: n8

Page 5: Advice Area
-Error(t0)
*/




/* Dash Functions: */

void uart3MessageReceived(void)
{
  /* If the message is to change the nextion page */
  if(uart_user_message[0] == 0x71 && uart_user_message[5] == 0xFF && uart_user_message[6] == 0xFF && uart_user_message[7] == 0xFF)
  {

	  blinkLed3();

    switch(uart_user_message[1])
    {
      case 0:
      actual_page = PAGE0;
      previous_page = PAGE0;
      NexPageShow(PAGE0);
      break;

      case 1:
      actual_page = PAGE1;
      previous_page = PAGE1;
      NexPageShow(PAGE1);
      break;

      case 2:
      actual_page = PAGE2;
      previous_page = PAGE2;
      NexPageShow(PAGE2);
      break;

      case 3:
      actual_page = PAGE3;
      previous_page = PAGE3;
      NexPageShow(PAGE3);
      break;

      case 4:
      actual_page = PAGE4;
      previous_page = PAGE4;
      NexPageShow(PAGE4);
      break;

      case 5:
      actual_page = PAGE5;
      previous_page = PAGE5;
      NexPageShow(PAGE5);
    }
  }
}


void nextionLoop(void)
{

//  if(can_vector[52].word_0 <= 13 && can_vector[52].word_0 != 0)  /* GLV Under Voltage */
//  {
//    actual_page = PAGE5;
//    actual_advice = GLV;
//  }
//  else if(can_vector[].word_ == 1)  /* BSE */
//  {
//    actual_page = PAGE5;
//    actual_advice = BSE;
//  }
//  else{  /* No Advice */
//    actual_advice = NO_ADVICE;
//  }
//
//  if(NO_ADVICE)

  switch(actual_page)
  {
    case PAGE0:

      break;
    
    case PAGE1:


      if(_flag_information_to_send == 0){
        NexNumberSetValue(0, can_vector[52].word_1);  // Battery Percent
      }

      else if(_flag_information_to_send == 1){
        NexProgressBarSetValue(0, can_vector[52].word_1); // Battery Percent
      }

      else if(_flag_information_to_send == 2){
        NexXfloatSetValue(1, can_vector[102].word_3); // Hodometer
      }

      else if(_flag_information_to_send == 3){
        NexXfloatSetValue(0, can_vector[105].word_0); // Speed
      }

      // else if(_flag_information_to_send == 4)
      //   NexNumberSetValue(3, brake_bias); // Brake Bias

      else if(_flag_information_to_send == 5){
        if(can_vector[102].word_1 && 0b0000000000001000 == 0)
          NexTextSetText(0, "OFF");
        else if(actual_advice == 0)
          NexTextSetText(0, "ON");
      }

      else if(_flag_information_to_send == 6){
        NexNumberSetValue(2, can_vector[106].word_0); // Torque
      }

      else{
        _flag_information_to_send = -1;
        blinkLed1();
      }


      break;


    case PAGE2:

      if(_flag_information_to_send == 0){
         NexNumberSetValue(4, can_vector[52].word_1);  // Battery Percent
      }

      else if(_flag_information_to_send == 1){
        NexProgressBarSetValue(1, can_vector[52].word_1); // Battery Percent
      }

      else if(_flag_information_to_send == 2){
        NexXfloatSetValue(0, can_vector[102].word_3); // Hodometer
      }

      else if(_flag_information_to_send == 3){
        NexXfloatSetValue(1, can_vector[105].word_3); // Speed
      }

//      else if(_flag_information_to_send == 4){
//        NexNumberSetValue(3, brake_bias); // Brake Bias
//      }

      else if(_flag_information_to_send == 5){
        if(can_vector[102].word_1 && 0b0000000000001000 == 0)
          NexTextSetText(0, "OFF");
        else if(actual_advice == 0)
          NexTextSetText(0, "ON");
      }

      else if(_flag_information_to_send == 6){
        NexNumberSetValue(0, can_vector[106].word_0); // Torque
      }

      else if(_flag_information_to_send == 7){
        if(can_vector[104].word_3 == 3)   /* Regen Brake High Level */
          NexProgressBarSetValue(0, 100);
        else if(can_vector[104].word_3 == 2) /* Regen Brake Medium Level */
          NexProgressBarSetValue(0, 67);
        else if(can_vector[104].word_3 == 1) /* Regen Brake Low Level */
          NexProgressBarSetValue(0, 34);
        else  /* Regen Brake off */
          NexProgressBarSetValue(0, 0);
      }

      // else if(_flag_information_to_send == 8){

        // BARRA DE LEDS DE EFICIENCIA

      //   if(motor.efficiency > )
      //     NexPictureSetPic(1, 9);
      //   else
      //     NexPictureSetPic(1, 10);
      //}

      // else if(_flag_information_to_send == 9){

       //CARGA POR DISTÂNCIA

      // }

      else{
        _flag_information_to_send = -1;
        blinkLed1();
      }

      break;

    case PAGE3:
    
      if(_flag_information_to_send == 0){
        NexXfloatSetValue(0, can_vector[53].word_3); /* Max Temperature */
      }

      else if(_flag_information_to_send == 1){
        NexXfloatSetValue(1, can_vector[54].word_0); /* Min Voltage */
      }

      else if(_flag_information_to_send == 2){
        NexXfloatSetValue(2, can_vector[53].word_1); /* Total Voltage */
      }

      else if(_flag_information_to_send == 3){    /* Mode */

        if(can_vector[102].word_0 == 4)
          NexTextSetText(0, "Enduro");
        else if(can_vector[102].word_0 == 1)
          NexTextSetText(0, "Accel");
        else if(can_vector[102].word_0 == 3)
          NexTextSetText(0, "Autoxx");
        else if(can_vector[102].word_0 == 2)
          NexTextSetText(0, "Skidpad");
        else if(can_vector[102].word_0 == 0)
          NexTextSetText(0, "Reverse");
      }

      else if(_flag_information_to_send == 4){  /* Status ECU */
        if(can_vector[1].word_2 && 0b0000000000000001 == 1)
          NexTextSetText(1, "APPS ERROR"); 
        else if(can_vector[1].word_2 && 0b0000000000000010 == 2)
          NexTextSetText(1, "Falha Comunicação");
        else NexTextSetText(1, "OK");
      }

      else if(_flag_information_to_send == 5){ /* GLV Voltage */
        NexXfloatSetValue(3, can_vector[52].word_0/100);
      }

      else if(_flag_information_to_send == 6){ /* Total Hodometer */
        NexXfloatSetValue(4, can_vector[102].word_3); 
      }

      else if(_flag_information_to_send == 7){ /* Status Datalog */
        if(can_vector[162].word_3 == 0) NexTextSetText(2, "ERROR");  
        else  NexTextSetText(2, "OK");
      }

      else if(_flag_information_to_send == 8){
        NexNumberSetValue(2, 0);  /* Rocker */
      }

      else if(_flag_information_to_send == 9){
        NexNumberSetValue(0, can_vector[REAL_CLK_CAN_ID].word_2); /* Hour */
      }

      else if(_flag_information_to_send == 10){
        NexNumberSetValue(1, can_vector[REAL_CLK_CAN_ID].word_3); /* Minute */
      }

      else{
        _flag_information_to_send = -1;
        blinkLed1();
      }

      break;
  

    case PAGE4:

      if(_flag_information_to_send == 0){
        NexNumberSetValue(0, can_vector[278].word_0);
      }

      else if(_flag_information_to_send == 1){
        NexNumberSetValue(5, can_vector[278].word_1);
      }

      else if(_flag_information_to_send == 2){
        NexNumberSetValue(6, can_vector[278].word_2);
      }

      else if(_flag_information_to_send == 3){
        NexNumberSetValue(1, can_vector[351].word_0);
      }

      else if(_flag_information_to_send == 4){
        NexNumberSetValue(4, can_vector[351].word_1);
      }

      else if(_flag_information_to_send == 5){
        NexNumberSetValue(7, can_vector[351].word_2);
      }

      else if(_flag_information_to_send == 6){
        NexNumberSetValue(2, can_vector[352].word_0);
      }

      else if(_flag_information_to_send == 7){
        NexNumberSetValue(3, can_vector[352].word_1);
      }

      else if(_flag_information_to_send == 8){
        NexNumberSetValue(8, can_vector[352].word_2);
      }

      else{
        _flag_information_to_send = -1;
        blinkLed1();
      }

      break;

    case PAGE5:
      
      if(actual_advice == GLV){
        NexTextSetText(0, "GLV");
      }

      else if(actual_advice == BSE){
        NexTextSetText(0, "BSE");
      }
      
      else{
        actual_page = previous_page;
        NexPageShow(actual_page);
      }

  }

  _flag_information_to_send++;

}


void nextionTestLoop(void)
{

  switch(actual_page)
  {
    case PAGE1:

      if(_flag_information_to_send == 0)
        NexProgressBarSetValue(0, auxiliar);
      else if(_flag_information_to_send == 1)
        NexNumberSetValue(0, auxiliar);
      else if(_flag_information_to_send == 2)
        NexXfloatSetValue(1, auxiliar);
      else if(_flag_information_to_send == 3)
        NexXfloatSetValue(0, auxiliar);
      else if(_flag_information_to_send == 4)
        NexNumberSetValue(3, auxiliar);
      else if(_flag_information_to_send == 5)
        NexNumberSetValue(2, auxiliar);
      else if(_flag_information_to_send == 6)
        NexNumberSetValue(1, auxiliar);
      else _flag_information_to_send = 0;

      break;

    case PAGE2:

      if(_flag_information_to_send == 0)
      {
        if(aux == 0){
          NexPictureSetPic(1, 9);
        }else if(aux == 2){
          NexPictureSetPic(1, 10);
        }
      }
      else if(_flag_information_to_send == 1)
        NexProgressBarSetValue(1, auxiliar);
      else if(_flag_information_to_send == 2)
        NexNumberSetValue(4, auxiliar);
      else if(_flag_information_to_send == 3)
        NexXfloatSetValue(0, auxiliar);
      else if(_flag_information_to_send == 4){
        if(aux == 0){
          NexProgressBarSetValue(0, 0);
        }else if(aux == 1){
          NexProgressBarSetValue(0, 34);
        }else if(aux == 2){
          NexProgressBarSetValue(0, 67);
        }else NexProgressBarSetValue(0, 100);
      }
      else if(_flag_information_to_send == 5)
        NexXfloatSetValue(1, auxiliar);
      else if(_flag_information_to_send == 6)
        NexNumberSetValue(3, auxiliar);
      else if(_flag_information_to_send == 7)
        NexNumberSetValue(2, auxiliar);
      else if(_flag_information_to_send == 8)
        NexNumberSetValue(0, auxiliar);
      else if(_flag_information_to_send == 9)
      {
        if(aux == 0){
          NexPictureSetPic(0, 5);
        }else if(aux == 1){
          NexPictureSetPic(0, 39);
        }else if(aux == 2){
          NexPictureSetPic(0, 40);
        }else NexPictureSetPic(0, 6);
      }

      if(aux == 9) aux = 0;
      
      blinkLed2();
      break;

    case PAGE3:

      if(_flag_information_to_send == 0)
        NexXfloatSetValue(0, auxiliar);
      else if(_flag_information_to_send == 1)
        NexXfloatSetValue(1, auxiliar);
      else if(_flag_information_to_send == 2)
        NexXfloatSetValue(2, auxiliar);
      else if(_flag_information_to_send == 3){
        if(aux == 0)  NexTextSetText(0, "Enduro");
        else  if(aux == 1)  NexTextSetText(0, "Accel");
        else  if(aux == 2)  NexTextSetText(0, "Autoxx");
        else  NexTextSetText(0, "Skidped");}
      else if(_flag_information_to_send == 4){
        if(aux == 0)  NexTextSetText(1, "OK");
        else  if(aux == 2)  NexTextSetText(1, "Deu Pau");}
      else if(_flag_information_to_send == 5)
        NexXfloatSetValue(3, auxiliar);
      else if(_flag_information_to_send == 6)
        NexXfloatSetValue(4, auxiliar);
      else if(_flag_information_to_send == 7){
        if(aux == 0)  NexTextSetText(2, "OK");
        else  if(aux == 2)  NexTextSetText(2, "Deu Pau");}
      else if(_flag_information_to_send == 8)
        NexNumberSetValue(2, auxiliar);
      else if(_flag_information_to_send == 9)
        NexNumberSetValue(0, auxiliar);
      else if(_flag_information_to_send == 10)
        NexNumberSetValue(1, auxiliar);
      else  _flag_information_to_send = 0;

    case PAGE4:

    if(_flag_information_to_send == 0)  
      NexNumberSetValue(0, auxiliar);
    else if(_flag_information_to_send == 1)
      NexNumberSetValue(1, auxiliar);
    else if(_flag_information_to_send == 2)
      NexNumberSetValue(2, auxiliar);
    else if(_flag_information_to_send == 3)
      NexNumberSetValue(3, auxiliar);
    else if(_flag_information_to_send == 4)
      NexNumberSetValue(4, auxiliar);
    else if(_flag_information_to_send == 5)
      NexNumberSetValue(5, auxiliar);
    else if(_flag_information_to_send == 6)
      NexNumberSetValue(6, auxiliar);
    else if(_flag_information_to_send == 7)
      NexNumberSetValue(7, auxiliar);
    else if(_flag_information_to_send == 8)
      NexNumberSetValue(8, auxiliar);
    else  _flag_information_to_send = 0;
    
      break;

    case PAGE5:

      if(aux == 0)  NexTextSetText(0, "GLV");
      else  if(aux == 2)  NexTextSetText(2, "BSE");
      break;

  }

  //VARIAVEIS PARA TESTE:
  actual_timer = HAL_GetTick();
  if(actual_timer - previous_timer > 200)
  {
    aux++;
    if(aux == 4)  aux = 0;
    previous_timer = HAL_GetTick();
  }

  if(auxiliar == 99)	auxiliar = 0;
  else	auxiliar++;

  _flag_information_to_send++;
}
