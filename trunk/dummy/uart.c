/**
 * @group: S11-G22
 * @file: uart.c
 * @date: 2011/06/27
 * @brief: UART related functions, ISR, and processes
 */

#include "rtx_inc.h"
#include "defs.h"
#include "dbug.h"
#include "util.h"
#include "process.h"
#include "envelope.h"
#include "memory.h"

#include "uart.h"

/**
 * @brief: Initializes the uart for serial IO 
 */
void uart_init()
{

	TRACE (" in uart init");
	UINT32 mask;

	/* Disable all interrupts */
	asm ( "move.w #0x2700,%sr" );

	/* Store the serial ISR at user vector #64 */
	asm( "move.l #asm_uart_entry,%d0" );
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

/**
 * @brief: ISR for handling uart interrupts. Loads the uart I-process
 */
void c_uart_handler()
{
	TRACE("c_uart_handler()\r\n");
	/* TODO: Load uart_iprocess */
	scheduler_run();
	return;
}

/**
 * @brief: Process responsible for performing uart IO operations
 */
void uart_iprocess()
{
	TRACE("uart_iprocess()\r\n");

	/* Acknowledge interrupt */
	BYTE temp = SERIAL1_USR;

	if (temp & 1)
	{
		TRACE("Receiving character...\r\n");

		/* Non-blocking call to k_request_memory_block() */
		void *mem_block = k_request_memory_block();
		*(char *)(mem_block + 64) = SERIAL1_RD;
		k_send_message(KCD_PROCESS_ID, mem_block);
	}
	else if (temp & 4)
	{
		TRACE("Transmitting character...\r\n");

		/* Non-blocking call to k_receive_mesasge() */
		struct envelope	*e = (struct envelope *)k_receive_message(NULL);
		
		if (e != NULL)
			SERIAL1_WD = *(char *)e->message;
	}

	k_release_processor();
}

/**
 * @brief: Keyboard command decoder process responsible for keyboard
 * 	input and command registration 
 */
void kcd_process()
{
	/* Initialize list to store command registrations */
	struct keyboard_command *head = NULL;
	struct keyboard_command *tail = NULL;

	while (TRUE)
	{
		struct envelope *e = (struct envelope *)receive_message(NULL);

		if (e->message_type == COMMAND_REGISTRATION)
		{
			/* TODO: add error check? */
			/* Construct a new keyboard_command struct */
			struct keyboard_command *kc = (struct keyboard_command *)malloc(sizeof(struct keyboard_command));
			*kc->command_identifier = *(char *)e->message;
			kc->registrant_process_ID = e->sender_process_ID;
			kc->next = NULL;

			/* Add to command registration list */
			if (head == NULL)
				head = kc;
			else
				tail->next = kc;

			tail = kc;
		}
		else if (e->message_type == KEYBOARD_INPUT)
		{
			/* TODO: Check if the string input in the message starts with a COMMMAND_PROMPT
			 * - If no then send the string to the CRT display process for output
			 * - If yes then send string to registered process in addition to the CRT display process 
			 */
			
			/* TODO: Send message to CRT display process */

			char *first_char = (char *)e->message;
			if (*first_char == COMMAND_PROMPT)
			{
				/* TODO: Send message to registered process */
			}
		}
	}
}

/**
 * @brief: CRT display process responsible for displaying text to screen
 */
void crt_display_process()
{
	while(TRUE)
	{
		struct envelope *e = (struct envelope *)receive_message(NULL);
		/* Extract character string from message */
		release_memory_block((void *)e);
		/* Loop through the characters in the string, sending each character to 
		 * the uart_iprocess for transmission to uart1 */
	}
}
