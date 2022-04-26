/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"

#include "gpio.h"



/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 9;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SJW = CAN_SJW_1TQ;
  hcan.Init.BS1 = CAN_BS1_6TQ;
  hcan.Init.BS2 = CAN_BS2_1TQ;
  hcan.Init.TTCM = DISABLE;
  hcan.Init.ABOM = ENABLE;
  hcan.Init.AWUM = DISABLE;
  hcan.Init.NART = DISABLE;
  hcan.Init.RFLM = DISABLE;
  hcan.Init.TXFP = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    /**CAN GPIO Configuration    
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_CAN1_2();

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN GPIO Configuration    
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_HP_CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/*Configuring the CAN Filter:*/
void CAN_Config_Filter(void){

	uint32_t filter_id = 0x00000000;
  uint32_t filter_mask = 0x00000000;
	//uint32_t filter_mask = 0xFFFFFF00;  //256
	CAN_FilterConfTypeDef filter;

  filter.FilterIdHigh = ((filter_id << 5)  | (filter_id >> (32 - 5))) & 0xFFFF; // STID[10:0] & EXTID[17:13]
  filter.FilterIdLow = (filter_id >> (11 - 3)) & 0xFFF8; // EXID[12:5] & 3 Reserved bits
  filter.FilterMaskIdHigh = ((filter_mask << 5)  | (filter_mask >> (32 - 5))) & 0xFFFF;
  filter.FilterMaskIdLow = (filter_mask >> (11 - 3)) & 0xFFF8;
  filter.FilterFIFOAssignment = 0;
  filter.FilterNumber = 0;
  filter.FilterMode = CAN_FILTERMODE_IDMASK;
  filter.FilterScale = CAN_FILTERSCALE_32BIT;
  filter.FilterActivation = ENABLE;
  filter.BankNumber = 14;

	if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK)  // RETORNA O STATUS DA FUNCAO
	{
	  _Error_Handler(__FILE__, __LINE__);
	}
}

void CAN_Config_Frames(void){
	static CanTxMsgTypeDef        TxMessage; // Struct de definicao da estrutura da mensagem CAN Tx
	static CanRxMsgTypeDef        RxMessage;

	initRxMes(&RxMessage);

	hcan.pTxMsg = &TxMessage;
	hcan.pRxMsg = &RxMessage;

	/*Configure Transmission process:*/

	// CONFIGURA A STRUCT TxMessage
	hcan.pTxMsg->StdId = 0x300; //Specifies the standard identifier
	hcan.pTxMsg->ExtId = 0x01; //Specifies the extended identifier.
	hcan.pTxMsg->RTR = CAN_RTR_DATA; //FRAME DE DADO|FRAME REMOTA / Specifies the type of frame for the message that will be transmitted.
	hcan.pTxMsg->IDE = CAN_ID_STD;//STANDARD ID 11b|EXTENDED ID 29b /Specifies the type of identifier for the message that will be transmitted.
	hcan.pTxMsg->DLC = 8; //Specifies the length of the frame that will be transmitted.
}

/*Start the Reception process and enable reception interrupt*/
void CAN_Receive_IT(void){
	if (HAL_CAN_Receive_IT(&hcan, CAN_FIFO0) != HAL_OK)
	{
	  /* Reception Error */
	  Error_Handler();
	}
}

void CAN_Transmit(uint8_t *vet, uint32_t id){

	hcan.pTxMsg->StdId = id; //Specifies the standard identifier
	for(uint8_t i=0; i < hcan.pTxMsg->DLC; i++)
		hcan.pTxMsg->Data[i] = vet[i];

	/*Start the Transmission process:*/
	HAL_StatusTypeDef trans_status = HAL_CAN_Transmit_IT(&hcan);

	//Error handler
	if (trans_status != HAL_OK)
	{
		//Error_Handler();
	}
}

void CAN_Transmit2(CanIdData_t *can_vector, uint32_t id){

	hcan.pTxMsg->StdId = id; //Specifies the standard identifier
//	for(uint8_t i=0; i < hcan.pTxMsg->DLC; i++)
	hcan.pTxMsg->Data[0] = can_vector[id].word_0;
	hcan.pTxMsg->Data[1] = can_vector[id].word_0 << 8;
	hcan.pTxMsg->Data[2] = can_vector[id].word_1;
	hcan.pTxMsg->Data[3] = can_vector[id].word_1 << 8;
	hcan.pTxMsg->Data[4] = can_vector[id].word_2;
	hcan.pTxMsg->Data[5] = can_vector[id].word_2 << 8;
	hcan.pTxMsg->Data[6] = can_vector[id].word_3;
	hcan.pTxMsg->Data[7] = can_vector[id].word_3 << 8;
	/*Start the Transmission process:*/
	HAL_StatusTypeDef trans_status = HAL_CAN_Transmit_IT(&hcan);

	//Error handler
	if (trans_status != HAL_OK)
	{
		//Error_Handler();
	}
}



/**
  * @brief  Initializes a Rx Message.
  * @param  CanRxMsgTypeDef *RxMessage
  * @retval None
  */

void initRxMes(CanRxMsgTypeDef *RxMessage)
{
  uint8_t i = 0;

  RxMessage->StdId = 0x321;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (i = 0;i < 8;i++)
  {
    RxMessage->Data[i] = 0x00;
  }
}

//void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef * hcan){}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef * hcan)
{
	/* For CAN Rx frames received in FIFO number 0 */
  __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV0);
  HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
  __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO0);

	/* For CAN Rx frames received in FIFO number 1 */
  //__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV1);
  //__HAL_CAN_Receive_IT(hcan, CAN_FIFO1);
	__HAL_CAN_RESET_HANDLE_STATE(hcan);
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_EWG | CAN_IT_EPV | CAN_IT_BOF | CAN_IT_LEC | CAN_IT_ERR | CAN_IT_FMP0| CAN_IT_FOV0| CAN_IT_FMP1| CAN_IT_FOV1| CAN_IT_TME);
	__HAL_UNLOCK(hcan);
}

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
