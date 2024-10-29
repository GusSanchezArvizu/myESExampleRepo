/****************************************************************************************************/
/**
\file       
\brief      
   \version       1.0
   \author        
   \description   
   \date          
*/

/** Scheduler function prototypes definitions */
#include "Uart.h"
#include "uart.h"
#include "MemAlloc.h"


/*****************************************************************************************************
* Defines - 
*****************************************************************************************************/

#define UART_CHANNEL_UNDEF (255)

#define UART_CH0  (0) 
#define UART_CH1  (1)
#define UART_CH2  (2)
#define UART_CH3  (3)
#define UART_CH4  (4)

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/

/* Uart Status Structure Example */
UartStatusType *UartStatus;
  
/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/
/* Array of Uart Register Base Address */
static const Uart * UartRegAddr[]={ UART0, UART1, UART2, UART3, UART4 };
static const uint32_t UartIDs[] = { ID_UART2, ID_UART4, ID_UART3, ID_UART0, ID_UART1 };
static const uint8_t IRQn[]={ UART2_IRQn, UART4_IRQn, UART3_IRQn, 0, 0};

/*****************************************************************************************************
* Code of module wide Private FUNCTIONS
*****************************************************************************************************/

uint8_t Uart_GetLogChannel(uint8_t PhyChannel)
{
  uint8_t LogicalChannel = UART_CHANNEL_UNDEF; 
  uint8_t LocChIdx = 0; /* LocChIdx represent the logical channel */
  /* UART_CFG_CHANNELS represents the number of configured channels from configuration structure */
  do
  {
    if (UartStatus[LocChIdx].ChannelId == PhyChannel)
    {
      LogicalChannel = LocChIdx; 
    }
    LocChIdx++;
  }while( (UartStatus[LocChIdx-1].ChannelId != PhyChannel) && (LocChIdx < UART_CFG_CHANNELS) );
  return (LogicalChannel);
}

void UART_Configure(Uart *uart,
		uint32_t mode,
		uint32_t baudrate,
		uint32_t masterClock)
{
	/* Reset and disable receiver & transmitter*/
	uart->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX
		| UART_CR_RXDIS | UART_CR_TXDIS | UART_CR_RSTSTA;

	uart->UART_IDR = 0xFFFFFFFF;

	/* Configure mode*/
	uart->UART_MR = mode;

	/* Configure baudrate*/
	uart->UART_BRGR = (masterClock / baudrate) / 16;

	uart->UART_CR = UART_CR_TXEN | UART_CR_RXEN;

}

void UART_SetTransmitterEnabled(Uart *uart, uint8_t enabled)
{
	if (enabled) {
		uart->UART_CR = UART_CR_TXEN;
	} else {
		uart->UART_CR = UART_CR_TXDIS;
	}
}

void UART_SetReceiverEnabled(Uart *uart, uint8_t enabled)
{
	if (enabled) {
		uart->UART_CR = UART_CR_RXEN;
	} else {
		uart->UART_CR = UART_CR_RXDIS;
	}
}

void UART_EnableIt(Uart *uart,uint32_t mode)
{
	uart->UART_IER = mode;
}

/*****************************************************************************************************
* Code of module wide Public FUNCTIONS
*****************************************************************************************************/

/*****************************************************
 * Initialization Interface Modulue
 ****************************************************/
void Uart_Init(const UartConfigType* Config)
{
	//initializes the UART module
	Uart* LocUartReg;
	uint32_t Parity = 0;
	uint32_t Mode = 0;
	uint32_t Baudrate = 0;
	uint32_t ClockSource = 0;
	uint32_t Interrupt = 0;
	uint8_t LocChIdx = 0;
	uint8_t physicalUART = 100;

	UartStatus = (UartStatusType*)MemAlloc(sizeof(UartStatusType) * UART_CFG_CHANNELS);

	for (LocChIdx = 0; LocChIdx < UART_CFG_CHANNELS; LocChIdx++)
	{

		physicalUART = Uart_GetLogChannel(Config->UartChannel[LocChIdx].ChannelId);
		if (physicalUART == 100)
		{
			return;
		}


		LocUartReg = (Uart*)UartRegAddr[physicalUART];
		UartStatus[LocChIdx].ChannelId = Config->UartChannel[LocChIdx].ChannelId;
		UartStatus[LocChIdx].UartChannel = &(Config->UartChannel[LocChIdx]);

		PMC_EnablePeripheral(UartIDs[LocChIdx]);

		switch(Config->UartChannel[LocChIdx].Parity)
		{
			case (UART_PARITY_NO):
				Parity = UART_MR_PAR_NO;
				break;
			case (UART_PARITY_EVEN):
				Parity = UART_MR_PAR_EVEN;
				break;
			case (UART_PARITY_MARK):
				Parity = UART_MR_PAR_MARK;
				break;
			case (UART_PARITY_ODD):
				Parity = UART_MR_PAR_ODD;
				break;
			case (UART_PARITY_SPACE):
				Parity = UART_MR_PAR_SPACE;
				break;
		}

		switch(Config->UartChannel[LocChIdx].Mode)
		{
			case (UART_MODE_AUTO):
				Mode = UART_MR_CHMODE_AUTOMATIC;
				break;
			case (UART_MODE_NORMAL):
				Mode = UART_MR_CHMODE_NORMAL;
				break;
			case (UART_MODE_LOOPBACK):
				Mode = UART_MR_CHMODE_LOCAL_LOOPBACK;
				break;
		}

		Baudrate = Config->UartChannel[LocChIdx].Baudrate;
		if (Config->ClkSrc == 0)
		{
			ClockSource = BOARD_MCK;
		}
		else
		{
			ClockSource = Config->ClkSrc;
		}

		UART_Configure(LocUartReg, (Parity | Mode), Baudrate, ClockSource);

		switch(Config->UartChannel[LocChIdx].ChannelId)
		{
			case(ID_UART0):
				NVIC_ClearPendingIRQ(UART0_IRQn); //BASE_IRQ is UART0_IRQn
				NVIC_SetPriority(UART0_IRQn, 1);
				Uart_SetTxEnable(UART_CFG_PHY_CHANNEL0, 1);
				NVIC_EnableIRQ(UART0_IRQn);
			break;
			case(ID_UART1):
				NVIC_ClearPendingIRQ(UART1_IRQn); //BASE_IRQ is UART1_IRQn
				NVIC_SetPriority(UART1_IRQn, 1);
				Uart_SetTxEnable(UART_CFG_PHY_CHANNEL1, 1);
				NVIC_EnableIRQ(UART1_IRQn);
			break;
			case(ID_UART2):
				NVIC_ClearPendingIRQ(UART2_IRQn); //BASE_IRQ is UART2_IRQn
				NVIC_SetPriority(UART2_IRQn, 1);
				Uart_SetTxEnable(UART_CFG_PHY_CHANNEL2, 1);
				NVIC_EnableIRQ(UART2_IRQn);
			break;
			case(ID_UART3):
				NVIC_ClearPendingIRQ(UART3_IRQn); //BASE_IRQ is UART3_IRQn
				NVIC_SetPriority(UART3_IRQn, 1);
				Uart_SetTxEnable(UART_CFG_PHY_CHANNEL3, 1);  
				NVIC_EnableIRQ(UART3_IRQn);
			break;
			case(ID_UART4):
				NVIC_ClearPendingIRQ(UART4_IRQn); //BASE_IRQ is UART4_IRQn
				NVIC_SetPriority(UART4_IRQn, 1);
				Uart_SetTxEnable(UART_CFG_PHY_CHANNEL4, 1);
				NVIC_EnableIRQ(UART4_IRQn); 
			break;
		}
	}
}

Std_ReturnType Uart_SetBaudrate(uint8_t Cahnnel, uint32_t Baudrate)
{
    Uart* LocUartReg = UartRegAddr[Channel];
    LocUartReg->UART_BRGR = (BOARD_MCK / Baudrate) / 16;
    if (UartRegAddr[Channel]->UART_BRGR != (BOARD_MCK / Baudrate) / 16) {
        return  E_NOT_OK;
    }
    return E_OK;
}

/*****************************************************
 * Receive Interface Modulue
 ****************************************************/
void Uart_SetRxEnable(uint8_t Channel, uint32_t Enable)
{

}

uint8_t Uart_GetByte(uint8_t Channel)
{
  uint8_t dataReceived = 0;
  return dataReceived;
}


/*****************************************************
 * Transmit Interface Modulue
 ****************************************************/

void Uart_SetTxEnable(uint8_t Channel, uint32_t Enable)
{

}

void Uart_Send(uint8_t Channel)
{
  const Uart * LocUartReg = UartRegAddr[UartStatus[Channel].ChannelId];
  /* Example Code */
  UartStatus[Channel].Counter++;
}

Std_ReturnType Uart_SendByte(uint8_t Channel, uint8_t Byte)
{
 if(1) //Command successfully executed
 {
  return E_OK;
 }
 else
 {
  return E_NOT_OK;
 } 
}

Std_ReturnType Uart_SendBuffer(uint8_t Channel, uint8_t *Buffer, uint16_t lenght)
{
  if(1)//Command successfully executed
  {
    return E_OK;
  }
  else
  {
    return E_NOT_OK;
  }
}
/*****************************************************
 * Status Interface Modulue
 ****************************************************/
uint32_t Uart_GetStatus(uint8_t Channel)
{
  uint32_t Status = 0;
  return Status;
}

/*****************************************************************************************************
* Code of module wide Interrupt Handler FUNCTIONS
*****************************************************************************************************/

void Uart_EnableInt(uint8_t Channel, uint32_t IntMode, uint8_t Enable)
{
  
}

void Uart_Isr( uint8_t Channel )
{
  const Uart * LocUartReg = UartRegAddr[Channel];
  uint8_t LocUartLogicChannel = Uart_GetLogChannel(Channel);
  
  /* Example Code */
  /* UART_CFG_CHANNELS represents the number of configured channels from configuration structure */
  if (LocUartLogicChannel<UART_CFG_CHANNELS)
    UartStatus[LocUartLogicChannel].TriggerCounter++;
	    
}


void UART0_Handler(void)
{
	Uart_Isr(UART_CH0);
}


void UART1_Handler(void)
{
	Uart_Isr(UART_CH1);
}


void UART2_Handler(void)
{
	Uart_Isr(UART_CH2);
}


void UART3_Handler(void)
{
	Uart_Isr(UART_CH3);
}


void UART4_Handler(void)
{
	Uart_Isr(UART_CH4);
}