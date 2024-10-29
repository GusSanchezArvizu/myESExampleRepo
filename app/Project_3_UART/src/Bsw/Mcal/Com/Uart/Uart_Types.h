/*******************************************************************************/
/**
\file       Uart_Types.h
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/

#ifndef UART_TYPES
#define UART_TYPES

  /** Standard Types */
  #include "Std_Types.h"
   /* Uart Error */
  typedef enum
  {
    UART_ERROR_OVERRUN = 0,
 	  UART_ERROR_FRAMING,
 	  UART_ERROR_PARITY 
  }UartErrorType;

  typedef void (*UartNotification)(void);
  typedef void (*UartErrorNotification)(UartErrorType Error);
  
	/* UartChannelType example */
  typedef struct
  {
    uint8_t ChannelId;
    uint8_t ISREn;
    uint8_t Mode;
    uint8_t Parity;
    uint32_t Baudrate;
    UartNotification TxNotification;
    UartNotification RxNotification;
    UartErrorNotification ErrorNotification;
  }UartChannelType;

	/* UartStatusType example */
	typedef struct
	{
		uint8_t ChannelId;
		uint8_t Counter;
		uint8_t TriggerCounter;
		uint32_t SrcClk;
		UartChannelType* UartChannel;
	} UartStatusType;



/*============================================================================*/
#endif /* UART_TYPES */
