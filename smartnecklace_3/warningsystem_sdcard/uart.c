#include "declaration.h"
#include "uart.h"

void uart_init()
{
	/* Initialize USART1 at 9600 baud, TX: PB6, RX: PB7 */
    TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
}

void uart_printf(char *str)
{
	TM_USART_Puts(USART1, str);
}