/*Criado por Francisco Gomes - Kotchergenko Engenharia 2018*/

#include "nRF24.h"

#define HIGH 1
#define LOW 0

void writeRegister(unsigned int reg, unsigned int _data);
void writeRegisterManyTimes(unsigned int reg, unsigned int* _data, int nBytes);

void RF24disablePipe(unsigned int _pipe);
unsigned int RF24msgSize();
void resetReceiveFlag();
void enableChip();
void disableChip();
void selectSlave();
void deselectSlave();
void enterTXmode(unsigned int _pipe);
void exitTXmode();
void enterRXmode();

/*Vari�veis globais da biblioteca*/
int ce_pin;
int csn_pin;
unsigned int _defaultAddressPipe0[6] = {0x10, 0x10, 0x10, 0x10, 0x10};
unsigned int _defaultAddressPipe1[6] = {0x11, 0x10, 0x10, 0x10, 0x11};
unsigned int _defaultAddressPipe2[6] = {0x11, 0x10, 0x10, 0x10, 0x12};
unsigned int _defaultAddressPipe3[6] = {0x11, 0x10, 0x10, 0x10, 0x13};
unsigned int _defaultAddressPipe4[6] = {0x11, 0x10, 0x10, 0x10, 0x14};
unsigned int _defaultAddressPipe5[6] = {0x11, 0x10, 0x10, 0x10, 0x15};
unsigned int cachePipeAddress;
bool pipesActives[6] = {true, true, false, false, false, false};

bool inTxMode = false;
uint8_t usingPipe = -1;
uint8_t pipePacakgeSize[5] = {32, 32, 32, 32, 32};

SPI_HandleTypeDef *globalHSPI;
extern TIM_HandleTypeDef htim3;


/*Delay em microsegundos*/
void RF_delay_init(void){
	  TIM_ClockConfigTypeDef sClockSourceConfig;
	  TIM_MasterConfigTypeDef sMasterConfig;
	  htim3.Instance = TIM3;
	  htim3.Init.Prescaler = 1000;
	  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim3.Init.Period = 65535;
	  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }
}

void RF_delay(int multiplo){
	int i = 0;
	for(i = 1; i <= multiplo; i++){
		TIM3->CNT = 0;
		HAL_TIM_Base_Start(&htim3);
		while(TIM3->CNT == 0);
		HAL_TIM_Base_Stop(&htim3);
		TIM3->CNT = 0;
	}
}
/**/

void digitalWrite(uint8_t pin, bool highLow){
    switch(pin){
        case(2):
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, highLow);
            break;
        case(3):
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, highLow);
            break;
        case(4):
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, highLow);
            break;
        case(5):
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, highLow);
            break;
        case(6):
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, highLow);
            break;
        case(10):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, highLow);
            break;
        case(11):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, highLow);
            break;
        case(12):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, highLow);
            break;
        case(13):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, highLow);
            break;
        case(14):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, highLow);
            break;
        case(15):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, highLow);
            break;
        case(16):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, highLow);
            break;
        case(17):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, highLow);
            break;
        case(18):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, highLow);
            break;
        case(19):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, highLow);
            break;
        case(20):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, highLow);
            break;
        case(21):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, highLow);
            break;
        case(22):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, highLow);
            break;
        case(25):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, highLow);
            break;
        case(26):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, highLow);
            break;
        case(27):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, highLow);
            break;
        case(28):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, highLow);
            break;
        case(29):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, highLow);
            break;
        case(30):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, highLow);
            break;
        case(31):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, highLow);
            break;
        case(32):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, highLow);
            break;
        case(33):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, highLow);
            break;
        case(34):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, highLow);
            break;
        case(37):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, highLow);
            break;
        case(38):
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, highLow);
            break;
        case(39):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, highLow);
            break;
        case(40):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, highLow);
            break;
        case(41):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, highLow);
            break;
        case(42):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, highLow);
            break;
        case(43):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, highLow);
            break;
        case(45):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, highLow);
            break;
        case(46):
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, highLow);
            break;
        default:
            break;

    }
}

uint8_t SPIsend(uint8_t data2Send){
  uint8_t buf2Receive;
  HAL_SPI_TransmitReceive(globalHSPI, &data2Send, &buf2Receive, 1, 100);
  return buf2Receive;
}

void RF24SetRetries(uint8_t retr, uint16_t delay){
	writeRegister(SETUP_RETR, (((delay/250)-1) << ARD) | retr);
}

void RF24init(SPI_HandleTypeDef *HSPI, int ce, int csn, uint8_t channel, unsigned int baudrateOverTheAir, unsigned int transmitPower) {
  unsigned int aux;
  unsigned int *address;
  //unsigned int baudrateOverTheAir = _1Mbps;
  //unsigned int transmitPower = _6dBm;
  unsigned int retries = _15retries;
  unsigned int retriesTimeout = _750us;

  RF_delay_init();

  ce_pin = ce;
  csn_pin = csn;
  globalHSPI = HSPI;

  disableChip();
  deselectSlave();

  //Set retries = 15 and timeout = 1500us
  RF24SetRetries(15, 750);
  //writeRegister(SETUP_RETR, (retriesTimeout << ARD) | (retries << ARC));

  //Set tx power = -12dbm (0dBm = 1mW, -12dBm = 1mW*10^-1.2 = 63uW)
  aux = readRegister(RF_SETUP);   //read current value
  aux &= ~(0b11 << RF_PWR);       //reset power values
  aux |= (transmitPower << RF_PWR); //set new value
  writeRegister( RF_SETUP, aux ); //write register

  /*//set datarate (1Mbps default)
  aux = readRegister(RF_SETUP);
  aux &= ~((~baudrateOverTheAir) << RF_DR_HIGH);
  writeRegister( RF_SETUP, aux );*/

  //set datarate (1Mbps default)
  aux = readRegister(RF_SETUP);
  aux &= ~(1 << RF_DR_HIGH);       //set to 1Mbps
  aux &= ~(1 << RF_DR_LOW);
  if(baudrateOverTheAir == _250kbps){
  	aux |= 1<<RF_DR_LOW;
   }
   else if(baudrateOverTheAir == _2Mbps){
	   	  aux |= (1 << RF_DR_HIGH);
    	  aux &= ~(1<<RF_DR_LOW);
   }
   writeRegister( RF_SETUP, aux );

  //set dynamicpayload off
  writeRegister(DYNPD, 0);

  //reset receive, transmit and max retransmits flags
  writeRegister(STATUS, 1 << RX_DR | 1 << TX_DS | 1 << MAX_RT);

  //set IRQ pin to set when receive data
  aux = readRegister(CONFIG) | 1 << MASK_TX_DS | 1 << MASK_MAX_RT | 1 << MASK_RX_DR; // Removida tamb�m a interrup��o de RX data ready
  writeRegister(CONFIG, aux) ;

  //set CRC enabled
  aux = readRegister(CONFIG) | 1 << CRCO | 1 << EN_CRC; //CRCO define 2 bytes of CRC
  writeRegister(CONFIG, aux) ;

  //set channel (76 is the default)
  writeRegister(RF_CH, channel < 125 ? channel : 125);

  //set _pipe active
  pipesActives[0] = true;
  writeRegister(EN_RXADDR, pipesActives[ERX_P0] << ERX_P0 | pipesActives[ERX_P1] << ERX_P1 | pipesActives[ERX_P2] << ERX_P2 | pipesActives[ERX_P3] << ERX_P3 | pipesActives[ERX_P4] << ERX_P4 | pipesActives[ERX_P5] << ERX_P5);

  //set pipes
  writeRegisterManyTimes(TX_ADDR, _defaultAddressPipe0, 5);

  address = _defaultAddressPipe0;
  writeRegisterManyTimes(RX_ADDR_P0, address, 5);
  writeRegister(RX_PW_P0, pipePacakgeSize[0]);

  address = _defaultAddressPipe1;
  writeRegisterManyTimes(RX_ADDR_P1, address, 5);
  writeRegister(RX_PW_P1, pipePacakgeSize[1]);

  address = _defaultAddressPipe2;
  writeRegister(RX_ADDR_P2, address[4]);
  writeRegister(RX_PW_P2, pipePacakgeSize[2]);

  address = _defaultAddressPipe3;
  writeRegister(RX_ADDR_P3, address[4]);
  writeRegister(RX_PW_P3, pipePacakgeSize[3]);

  address = _defaultAddressPipe4;
  writeRegister(RX_ADDR_P4, address[4]);
  writeRegister(RX_PW_P4, pipePacakgeSize[4]);

  address = _defaultAddressPipe5;
  writeRegister(RX_ADDR_P5, address[4]);
  writeRegister(RX_PW_P5, pipePacakgeSize[5]);

  //stop listening
  disableChip();
  writeRegister(FLUSH_TX, NOP);
  writeRegister(FLUSH_RX, NOP);

  //enable dynamic payload and ack + payload
  writeRegister(FEATURE, readRegister(FEATURE) | 1<<EN_DPL | 1<<EN_ACK_PAY);
  writeRegister(DYNPD, 0x3F);

  enterRXmode();

  aux = readRegister(RF_SETUP);   //read current value

}

uint8_t RF24send(char* _data, unsigned int _size, unsigned int _pipe) {

  if(_size > 32)
	  _size = 32;

  if(!inTxMode){
	  inTxMode = true;
	  usingPipe = _pipe;
	  enterTXmode(_pipe);
	  //enableChip();
  }
  else{
	  if(usingPipe != _pipe){
		  usingPipe = _pipe;
		  enterTXmode(_pipe);
          //enableChip();
	  }
	  RF_delay(6);
      writeRegister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );
  }

  selectSlave();

  SPIsend(W_TX_PAYLOAD);
  while (_size--)
	SPIsend(*_data++);

  deselectSlave();

  enableChip();
  RF_delay(10);	//	Alterado de 30 para 10
  disableChip();

  return RF24msgSize();

}

void RF24sendInTheNextReceive(char* _data, unsigned int _size, unsigned int _pipe){
	if(_size >32)
		_size = 32;

	uint8_t blank_len = 32 - _size;

	selectSlave();

	SPIsend(W_ACK_PAYLOAD | _pipe);
	while (_size--)
		SPIsend(*_data++);

	deselectSlave();
}

void RF24Flush(){
	writeRegister(FLUSH_TX, NOP);
	writeRegister(FLUSH_RX, NOP);
}

void RF24read(char* _data) {
  int _size;

  _size = readRegister(R_RX_PL_WID);

  selectSlave();

  SPIsend(R_RX_PAYLOAD);
  for (int i = 0; i < _size; i++)
    _data[i] = SPIsend(NOP);

  deselectSlave();

  resetReceiveFlag();

}

void RF24attrAddress2Pipe0(unsigned int* address){
  _defaultAddressPipe0[0] = address[0];
  _defaultAddressPipe0[1] = address[1];
  _defaultAddressPipe0[2] = address[2];
  _defaultAddressPipe0[3] = address[3];
  _defaultAddressPipe0[4] = address[4];
  _defaultAddressPipe0[5] = address[5];

  writeRegister(CONFIG, readRegister(CONFIG) | (1<<PWR_UP) | (1<<PRIM_RX));
  writeRegister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );

  if (!pipesActives[0])
    RF24enablePipe(0);

  writeRegisterManyTimes(TX_ADDR, address, 5);
  writeRegisterManyTimes(RX_ADDR_P0, address, 5);
  writeRegister(RX_PW_P0, pipePacakgeSize[0]);

  writeRegister(FLUSH_TX, NOP);
  writeRegister(FLUSH_RX, NOP);
  enableChip();
  RF_delay(1);
}

void RF24attrAddress2Pipe1(unsigned int* address){
  _defaultAddressPipe1[0] = address[0];
  _defaultAddressPipe1[1] = address[1];
  _defaultAddressPipe1[2] = address[2];
  _defaultAddressPipe1[3] = address[3];
  _defaultAddressPipe1[4] = address[4];

  _defaultAddressPipe2[0] = address[0];
  _defaultAddressPipe2[1] = address[1];
  _defaultAddressPipe2[2] = address[2];
  _defaultAddressPipe2[3] = address[3];
  _defaultAddressPipe2[4] = address[4];

  _defaultAddressPipe3[0] = address[0];
  _defaultAddressPipe3[1] = address[1];
  _defaultAddressPipe3[2] = address[2];
  _defaultAddressPipe3[3] = address[3];
  _defaultAddressPipe3[4] = address[4];

  _defaultAddressPipe4[0] = address[0];
  _defaultAddressPipe4[1] = address[1];
  _defaultAddressPipe4[2] = address[2];
  _defaultAddressPipe4[3] = address[3];
  _defaultAddressPipe4[4] = address[4];

  _defaultAddressPipe5[0] = address[0];
  _defaultAddressPipe5[1] = address[1];
  _defaultAddressPipe5[2] = address[2];
  _defaultAddressPipe5[3] = address[3];
  _defaultAddressPipe5[4] = address[4];

  _defaultAddressPipe1[5] = address[5];

  writeRegister(CONFIG, readRegister(CONFIG) | (1<<PWR_UP) | (1<<PRIM_RX));
  writeRegister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );

  if (!pipesActives[1])
    RF24enablePipe(1);

  writeRegisterManyTimes(TX_ADDR, address, 5);
  writeRegisterManyTimes(RX_ADDR_P1, address, 5);
  writeRegister(RX_PW_P1, 32);

  writeRegister(FLUSH_TX, NOP);
  writeRegister(FLUSH_RX, NOP);
  enableChip();
  RF_delay(1);
}

void RF24attrAddress2Pipe2(unsigned int address){
  _defaultAddressPipe2[4] = address;

  writeRegister(CONFIG, readRegister(CONFIG) | (1<<PWR_UP) | (1<<PRIM_RX));
  writeRegister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );

  if (!pipesActives[2])
    RF24enablePipe(2);

  writeRegisterManyTimes(TX_ADDR, _defaultAddressPipe2, 5);
  writeRegister(RX_ADDR_P2, _defaultAddressPipe2[4]);
  writeRegister(RX_PW_P2, 32);

  writeRegister(FLUSH_TX, NOP);
  writeRegister(FLUSH_RX, NOP);
  enableChip();
  RF_delay(1);
}

void RF24attrAddress2Pipe3(unsigned int address){
  _defaultAddressPipe3[4] = address;

  writeRegister(CONFIG, readRegister(CONFIG) | (1<<PWR_UP) | (1<<PRIM_RX));
  writeRegister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );

  if (!pipesActives[3])
    RF24enablePipe(3);

  writeRegisterManyTimes(TX_ADDR, _defaultAddressPipe3, 5);
  writeRegister(RX_ADDR_P3, _defaultAddressPipe3[4]);
  writeRegister(RX_PW_P3, 32);

  writeRegister(FLUSH_TX, NOP);
  writeRegister(FLUSH_RX, NOP);
  enableChip();
  RF_delay(1);
}

void RF24attrAddress2Pipe4(unsigned int address){
  _defaultAddressPipe4[4] = address;

  writeRegister(CONFIG, readRegister(CONFIG) | (1<<PWR_UP) | (1<<PRIM_RX));
  writeRegister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );

  if (!pipesActives[4])
    RF24enablePipe(4);

  writeRegisterManyTimes(TX_ADDR, _defaultAddressPipe4, 5);
  writeRegister(RX_ADDR_P4, _defaultAddressPipe4[4]);
  writeRegister(RX_PW_P4, 32);

  writeRegister(FLUSH_TX, NOP);
  writeRegister(FLUSH_RX, NOP);
  enableChip();
  RF_delay(1);
}

void RF24attrAddress2Pipe5(unsigned int address){
  _defaultAddressPipe5[4] = address;

  writeRegister(CONFIG, readRegister(CONFIG) | (1<<PWR_UP) | (1<<PRIM_RX));
  writeRegister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );

  if (!pipesActives[5])
    RF24enablePipe(5);

  writeRegisterManyTimes(TX_ADDR, _defaultAddressPipe5, 5);
  writeRegister(RX_ADDR_P5, _defaultAddressPipe5[4]);
  writeRegister(RX_PW_P5, 32);

  writeRegister(FLUSH_TX, NOP);
  writeRegister(FLUSH_RX, NOP);
  enableChip();
  RF_delay(1);
}

void enterRXmode() {

    disableChip();

	writeRegister(CONFIG, readRegister(CONFIG) | (1<<PWR_UP) | (1<<PRIM_RX));
	//writeRegister(CONFIG, 0x0F);
	writeRegister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );

	writeRegisterManyTimes(TX_ADDR, _defaultAddressPipe0, 5);
	writeRegisterManyTimes(RX_ADDR_P0, _defaultAddressPipe0, 5);
	writeRegister(RX_PW_P0, pipePacakgeSize[0]);

	writeRegister(FLUSH_TX, NOP);
	writeRegister(FLUSH_RX, NOP);
	enableChip();
	RF_delay(1);
}

void enterTXmode(unsigned int _pipe) {

  unsigned int* address;

  switch (_pipe) {
    case 0:
      cachePipeAddress = 0;
      address = _defaultAddressPipe0;
      break;
    case 1:
      cachePipeAddress = 1;
      address = _defaultAddressPipe1;
      break;
    case 2:
      cachePipeAddress = 2;
      address = _defaultAddressPipe2;
      break;
    case 3:
      cachePipeAddress = 3;
      address = _defaultAddressPipe3;
      break;
    case 4:
      cachePipeAddress = 4;
      address = _defaultAddressPipe4;
      break;
    case 5:
      cachePipeAddress = 5;
      address = _defaultAddressPipe5;
      break;
    default:
      cachePipeAddress = 0;
      address = _defaultAddressPipe0;
      break;
  }

  disableChip();

  if (!pipesActives[_pipe])
    RF24enablePipe(_pipe);

  writeRegister(CONFIG, 0x0E);
  writeRegister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );

  writeRegisterManyTimes(TX_ADDR, address, 5);
  writeRegisterManyTimes(RX_ADDR_P0, address, 5);
  writeRegister(RX_PW_P0, pipePacakgeSize[cachePipeAddress]);
  if(_pipe ==1){
	  writeRegisterManyTimes(RX_ADDR_P0 + _pipe, _defaultAddressPipe0, 5);
	}
	else if(_pipe >= 2 && _pipe <=5){
	  writeRegister(RX_ADDR_P0 + _pipe, _defaultAddressPipe0[4]);
	}

  writeRegister(FLUSH_TX, NOP);
  writeRegister(FLUSH_RX, NOP);
}

void exitTXmode(){

	writeRegisterManyTimes(TX_ADDR, _defaultAddressPipe0, 5);
	writeRegisterManyTimes(RX_ADDR_P0, _defaultAddressPipe0, 5);
	writeRegister(RX_PW_P0, pipePacakgeSize[0]);

	if(cachePipeAddress != 0){
        if(cachePipeAddress == 1)
            writeRegisterManyTimes(RX_ADDR_P1, _defaultAddressPipe1, 5);
        else if(cachePipeAddress == 2)
            writeRegister(RX_ADDR_P2, _defaultAddressPipe2[4]);
        else if(cachePipeAddress == 3)
            writeRegister(RX_ADDR_P3, _defaultAddressPipe3[4]);
        else if(cachePipeAddress == 4)
            writeRegister(RX_ADDR_P4, _defaultAddressPipe4[4]);
        else
            writeRegister(RX_ADDR_P5, _defaultAddressPipe5[4]);

        writeRegister(RX_PW_P0 + cachePipeAddress, pipePacakgeSize[cachePipeAddress]);
	}

}

void writeRegister(unsigned int reg, unsigned int _data) {

  selectSlave();
  SPIsend(W_REGISTER | reg);
  SPIsend(_data);
  deselectSlave();

}

void writeRegisterManyTimes(unsigned int reg, unsigned int* _data, int nBytes) {
	int i;
	selectSlave();
	SPIsend(W_REGISTER | reg);
	for (i = nBytes - 1; i >= 0; i--)
		SPIsend(_data[i]);
	deselectSlave();
}

unsigned int readRegister(unsigned int reg) {
  unsigned int _data;

  selectSlave();
  SPIsend(R_REGISTER | reg);
  _data = SPIsend(NOP);
  deselectSlave();

  return _data;
}

void readRegisterManyTimes(unsigned int reg, unsigned int* _data, int nBytes) {
  int index = 0;

  selectSlave();
  SPIsend(R_REGISTER | reg);
  while(nBytes--){
    _data[index] = SPIsend(NOP);
    index++;
  }
  deselectSlave();

}

//Returns false if didn't receive anything. Returns (_pipe_n +1) when receive anything from the _pipe number _pipe_n
volatile unsigned int RF24available() {
	if(inTxMode){
		inTxMode = false;
		exitTXmode();
		enterRXmode();
	}
    else{
        writeRegister(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );
    }
	if ((~readRegister(FIFO_STATUS)) & (1<<RX_EMPTY)) {
		unsigned int receivedFromPipe = (readRegister(STATUS) & 0b1110) >> 1;
		return receivedFromPipe + 1;
	}
	else
		return false;
}

uint8_t RF24checkRetries(){
	return readRegister(STATUS) & (1<<MAX_RT);
}

void RF24enablePipe(unsigned int _pipe) {
  pipesActives[_pipe] = true;
  writeRegister(EN_RXADDR, pipesActives[ERX_P0] << ERX_P0 | pipesActives[ERX_P1] << ERX_P1 | pipesActives[ERX_P2] << ERX_P2 | pipesActives[ERX_P3] << ERX_P3 | pipesActives[ERX_P4] << ERX_P4 | pipesActives[ERX_P5] << ERX_P5);
}

void RF24disablePipe(unsigned int _pipe) {
  pipesActives[_pipe] = false;
  writeRegister(EN_RXADDR, pipesActives[ERX_P0] << ERX_P0 | pipesActives[ERX_P1] << ERX_P1 | pipesActives[ERX_P2] << ERX_P2 | pipesActives[ERX_P3] << ERX_P3 | pipesActives[ERX_P4] << ERX_P4 | pipesActives[ERX_P5] << ERX_P5);
}

unsigned int RF24msgSize() {
	if ((~readRegister(FIFO_STATUS)) & (1<<RX_EMPTY)) {
		return readRegister(R_RX_PL_WID);
	}
	else{
		return 0;
	}
}

void resetReceiveFlag() {
  writeRegister(STATUS, 1 << RX_DR);
}

void enableChip() {
  digitalWrite(ce_pin, HIGH);
}

void disableChip() {
  digitalWrite(ce_pin, LOW);
}

void selectSlave(){
	digitalWrite(csn_pin, LOW);
}

void deselectSlave(){
	digitalWrite(csn_pin, HIGH);
}
