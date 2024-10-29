/****************************************************************************************************/
/**
  \mainpage
  \n 
  \brief        Main application (main module)
  \author       Abraham Tezmol Otero, M.S.E.E
  \project      Tau 
  \version      1.0
  \date         12/Jun/2016
   
  Program compiled with  WinIDEA Open Build 9.12.256 (54401), tested on Atmel SAMV71 Xplained Ultra
*/
/****************************************************************************************************/


/*~~~~~~  Headers ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Main group of includes for board definitions, chip definitions and type definitions */
#include    "Std_Types.h"
/** Task scheduler definitions */
#include    "SchM.h"
/** LED control definitions */ 
#include    "Led_Ctrl.h"
/** Watchdog control function prototypes definitions */
#include    "Wdg.h"
/** Button Control interfaces */
#include    "Button_Ctrl.h"
/** Uart interfaces */
#include    "Uart.h"


/*~~~~~~  Local definitions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~  Global variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** UART'S Defiennation */
#define PINS_UART0  \
		{PIO_PA9A_URXD0 | PIO_PA10A_UTXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PINS_UART1 \
		{PIO_PA5C_URXD1 | PIO_PA6C_UTXD1, PIOA, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT}
#define PINS_UART2 \
		{PIO_PD25C_URXD2 | PIO_PD26C_UTXD2, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
#define PINS_UART3 \
		{PIO_PD28A_URXD3 | PIO_PD30A_UTXD3, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}
#define PINS_UART4 \
		{PIO_PD18C_URXD4 | PIO_PD19C_UTXD4, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
		
const Pin pPins[] = { PINS_UART0, PINS_UART1, PINS_UART2, PINS_UART3, PINS_UART4 };


static const Pin UARTPins[] = {PINS_UART0,PINS_UART4};

/*~~~~~~  Local functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void TxNotification()
{
	printf("Uart: Ready to text\n");
}

void RxNotification()
{
	printf("Uart: Ready to read\n");
}

void ErrorNotification(UartErrorType Error)
{
	switch (Error)
	{
		case UART_ERROR_OVERRUN:
			printf("Uart Error Overrun\n");
			break;
		case UART_ERROR_FRAMING:
			printf("Uart Error Framing\n");
			break;
		case UART_ERROR_PARITY:
			printf("Uart Error Parity\n");
			break;
		default:
			printf("error: this ain't it, chief\n");
	}
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
/**
 *  \brief getting-started Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
extern int main( void )
{
	/* Disable watchdog */
	Wdg_Disable();
	printf( "\n\r-- Scheduler Project %s --\n\r", SOFTPACK_VERSION ) ;
	printf( "-- %s\n\r", BOARD_NAME ) ;
	printf( "-- Compiled: %s %s With %s --\n\r", __DATE__, __TIME__ , COMPILER_NAME);
	/* Enable I and D cache */
	SCB_EnableICache();
	/* SCB_EnableDCache(); */
	/* Configure LEDs */
	printf( "-- Led Control --\n\r" ) ;
	LedCtrl_Configure();
  
    printf( "-- Button Control --\n\r" ) ;  
    ButtonCtrl_ConfigureSW0Button();
  
    /* Uart Inititalization */
    printf( "-- Uart Initialization --\n\r" ) ;

	PIO_Configure(pPins, PIO_LISTSIZE(pPins));
	Uart_Init(&UartConfig[0]);

	Uart_SetBaudrate(UART_CFG_CHANNEL0, 9600);
	Uart_SetBaudrate(UART_CFG_CHANNEL1, 14400);
	Uart_SetBaudrate(UART_CFG_CHANNEL2, 38400);
	Uart_SetBaudrate(UART_CFG_CHANNEL3, 57600);
	Uart_SetBaudrate(UART_CFG_CHANNEL4, 9600);
  
	/* Scheduler Inititalization */
	printf( "-- Scheduler Initialization --\n\r" ) ;
	SchM_Init(ScheduleConfig);
	
	/* Should never reach this code */
	for(;;)
    {
		printf( "-- Unexpected Error at Scheduler Initialization --\n\r" ) ;
	}
}
