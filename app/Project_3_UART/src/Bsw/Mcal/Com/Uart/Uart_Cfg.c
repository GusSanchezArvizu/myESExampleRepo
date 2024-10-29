/*******************************************************************************/
/**
\file       Uart_Cfg.c
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/

#include "Uart.h"
#include "Uart_Ctrl.h"   

/* Callback functions */

/* Example of 3 configured channels */
const uint8_t UartConfiguredChannels[] = {
  UART_CFG_CHANNEL4,
  UART_CFG_CHANNEL0,
  UART_CFG_CHANNEL2
};

//This container contains the configuration parameters of the Uart channel
UartChannelType const UartCh[] = {
	{UART_CFG_CHANNEL0, UART_INT_TXRDY, UART_MODE_NORMAL, UART_PARITY_NO, 9600, vfnSerialCtrl_Transfer0, NULL, NULL},
	{UART_CFG_CHANNEL1, UART_INT_TXRDY, UART_MODE_NORMAL, UART_PARITY_NO, 14400, vfnSerialCtrl_Transfer1, NULL, NULL},
	{UART_CFG_CHANNEL2, UART_INT_TXRDY, UART_MODE_NORMAL, UART_PARITY_NO, 38400, vfnSerialCtrl_Transfer2, NULL, NULL},
	{UART_CFG_CHANNEL3, UART_INT_TXRDY, UART_MODE_NORMAL, UART_PARITY_NO, 57600, vfnSerialCtrl_Transfer3, NULL, NULL},
	{UART_CFG_CHANNEL4, UART_INT_TXRDY, UART_MODE_NORMAL, UART_PARITY_NO, 115200, vfnSerialCtrl_Transfer4, NULL, NULL},
};

// Configuration of the UART (Uart driver) module
UartConfigType UartConfig[] = {
	{
		sizeof(UartCh) / sizeof(UartChannelType),
		(uint8_t)UART_PER_CLK,
		&UartCh[0]
	}
}; 



