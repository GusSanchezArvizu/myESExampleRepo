/*******************************************************************************/
/**
\file       Uart.h
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/

#ifndef _UART_H        /*prevent duplicated includes*/
	#define _UART_H

	/** Standard Types */
	#include "Std_Types.h"
  #include "Uart_Types.h"
	#include "Uart_Cfg.h"

   /* interrupt configuration */
  #define UART_INT_DISABLED 0
  #define UART_INT_RXRDY 1
  #define UART_INT_TXRDY 2
  #define UART_INT_OVR_ERROR 32
  #define UART_INT_FRAME_ERROR 64
  #define UART_INT_PAR_ERROR 128
  #define UART_INT_TXEMPTY 512                                                            

  /* Uart Initialization Fucntion */
  extern void Uart_Init(const UartConfigType* Config);
  /* Uart Send Fucntion */
  extern void Uart_Send(uint8_t Channel);
  	extern Std_ReturnType Uart_SendBuffer(uint8_t Channel, uint8_t* Buffer, uint16_t Length);


  /* Example Code - Need to be removed */
  extern void UART0_Handler(void);
  extern void UART1_Handler(void);
  extern void UART2_Handler(void);
  extern void UART3_Handler(void);
  extern void UART4_Handler(void);

/*============================================================================*/
#endif /* _UART_H */