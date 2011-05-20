/*
 * @file: uart1.c
 * @brief: Output text to UART1
 * @group: S11-G022
 * @date: 2011/05/12
 */

#include "uart1.h"

/*
 * @brief: Initialize UART1
 */
VOID uart1_init()
{
	// Reset entire UART1
	SERIAL1_UCR = 0x10;

	// Reset transmitter
	SERIAL1_UCR = 0x30;

	// Reset error condition
	SERIAL1_UCR = 0x40;

	// Set the baud rate
	SERIAL1_UBG1 = 0x00;
#ifdef _CFSERVER_
	SERIAL1_UBG2 = 0x49;	// cf-server baud rate 19200
#else
	SERIAL1_UBG2 = 0x92;	// Lab board baud rate 9600
#endif /* _CFSERVER */

	// Set the clock mode
	SERIAL1_UCSR = 0xDD;

	// Setup UART1 for no parity, 8 bits
	SERIAL1_UMR = 0x13;

	// Setup UART1 for no echo, 1 stop bit
	SERIAL1_UMR = 0x07;

	// Enable transmitter
	SERIAL1_UCR = 0x04;
}

/*
 * @brief: Put the given character into the UART1 transmit register
 * @param: c The string to move to the register
 */
VOID print_char( CHAR c )
{
	// Poll transmit ready bit until it is set
	while ( !( SERIAL1_USR & 4 ) ) {}

	SERIAL1_WD = c;
}

/*
 * @brief: Print the given string by calling print_char()
 * @param: string Pointer to string
 */
VOID print_string( CHAR* string )
{
	if ( string == NULL )
	{
		return;
	}

	while ( *string != '\0' )
	{
		print_char( *string++ );
	}
}
