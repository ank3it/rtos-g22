/**
 * @group: S11-G22
 * @file: uart1.c
 * @brief: UART1 I/O
 * @group: S11-G022
 * @date: 2011/05/12
 */

#include "uart1.h"

/**
 * @brief: Setting hotkeys
 */
void hotkeys()
{
	CharIn = SERIAL1_RD;
	TRACE("\r\n");
	switch(CharIn)
	{		
		#ifdef _HOTKEYS_DEBUG
			case 'Q'
				TRACE("A hotkey was pressed");
				print_ready_queue();
				break;
			case 'W'
				TRACE("A hotkey was pressed");
				print_blocked_memory_queue();
				break;
			case 'E'
				TRACE("A hotkey was pressed");
				print_blocked_receive_queue();
				break;
			case 'R'
				TRACE("A hotkey was pressed");
				print_all_process_info();
				break;
			case 'T'
				TRACE("A hotkey was pressed");
				print_avail_memory_block_queue();
				break;			
			case 'Y'
				TRACE("A hotkey was pressed");
				print_message_queue();
				break;	
		#endif
	}
	
	TRACE("Sending message to KCD \r\n\");
	envelope *message_send = (envelope *)request_memory_block();
	message->message_data[0] = CharIn;
	send_message(KCD_ID, message_send);
}

/**
 * @brief: Initialize UART1 for I/O
 */
VOID uart1_init()
{
	/* Initialize variables */
	Caught = TRUE;
	CharIn = '!';
	CharOut = '\0';
	TransmitReady = TRUE;

	UINT32 mask;

	/* Disable all interrupts */
	asm ( "move.w #0x2700,%sr" );

	/* Store the serial ISR at user vector #64 */
	asm( "move.l #asm_serial_entry,%d0" );
	asm( "move.l %d0,0x10000100" );

	/* Reset entire UART1 */ 
	SERIAL1_UCR = 0x10;

	/* Reset the receiver */
	SERIAL1_UCR = 0x20;

	/* Reset transmitter */
	SERIAL1_UCR = 0x30;

	/* Reset error condition */
	SERIAL1_UCR = 0x40;

	/* Install the interrupt */
	SERIAL1_ICR = 0x17;
	SERIAL1_IVR = 64;

	/* Enable interrupts on rx only */
	SERIAL1_IMR = 0x02;

	/* Set the baud rate */
	SERIAL1_UBG1 = 0x00;
#ifdef _CFSERVER_
	SERIAL1_UBG2 = 0x49;	/* cf-server baud rate 19200 */
#else
	SERIAL1_UBG2 = 0x92;	/* Lab board baud rate 9600 */
#endif /* _CFSERVER */

	/* Set the clock mode */
	SERIAL1_UCSR = 0xDD;

	/* Setup UART1 for no parity, 8 bits */
	SERIAL1_UMR = 0x13;

	/* Setup UART1 for no echo, 1 stop bit */
	SERIAL1_UMR = 0x07;

	/* Enable transmit and receive */
	SERIAL1_UCR = 0x05;

	/* Mask UART1  interrupts */
	mask = SIM_IMR;
	mask &= 0x0003dfff;
	SIM_IMR = mask;

	/* Enable all interrupts */
	asm ( "move.w #0x2000,%sr" );
}

VOID c_serial_handler( VOID )
{
	BYTE temp;

	temp = SERIAL1_USR;		/* Acknowledge interrupt */
	
	if ( temp & 1 )
	{
		CharIn = SERIAL1_RD;
		Caught = FALSE;
	}
	else if ( temp & 4 )
	{
		SERIAL1_WD = CharOut;
		TransmitReady = TRUE;
		SERIAL1_IMR = 2;
	}

	return;
}

/**
 * @brief: Puts the given character in the UART1 transmitter buffer
 * @param: c Character to place into the buffer
 */
VOID print_char( CHAR c )
{
	CharOut = c;
	TransmitReady = FALSE;

	/* Enable tx interrupts */
	SERIAL1_IMR = 3;
}

/**
 * @brief: Iterates through a string, printing each character
 * @param: s String to be printed
 */
VOID print_string( CHAR *s )
{
	if ( s == NULL )
	{
		return;
	}

	while ( *s != '\0' )
	{
		if ( TransmitReady == TRUE )
		{
			print_char( *s++ );
		}
	}	
}
