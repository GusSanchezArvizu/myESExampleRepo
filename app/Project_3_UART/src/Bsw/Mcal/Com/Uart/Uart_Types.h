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

/** Error Notification Enum */
typedef enum UartErrorType
{
  UART_ERROR_OVERRUN =    0,
  UART_ERROR_FRAMING =    1,
  UART_ERROR_PARITY =     2
} UartErrorType;

/** Notification variables types */
typedef void (*UartNotification)(void);
typedef void (*UartErrorNotification)(UartErrorType Error);

/* UartStatusType example */
typedef struct
{
  uint8_t ChannelId;
  uint8_t Counter;
  uint8_t TriggerCounter;
} UartStatusType;
  
/* UartConfigType struct */
typedef struct UartConfigType
{
  uint8_t UartNumberOfChannels;
  uint8_t ClkSrc;
  UartChannelType* UartCahnnel;
} UartConfigType;

typedef struct UartChannelType
{
  uint8_t ChannelId;
  uint32_t IsrEn;
  uint8_t Mode;
  uint8_t Parity;
  uint32_t Baudrate;
  UartNotification TxNotification;
  UartNotification RxNotification;
  UartErrorNotification ErrorNotification;
} UartChannelType;
/*============================================================================*/
#endif /* UART_TYPES */
