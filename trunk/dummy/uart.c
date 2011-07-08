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
#include "rtx.h"
#include "hotkeys.h"

#include "uart.h"

volatile BYTE CharIn;


/**
 * @brief: Setting hotkeys
 */
void hotkeys()
{
	CharIn = SERIAL1_RD;
	rtx_dbug_outs("\r\n");
	rtx_dbug_outs("Entering switch");
	rtx_dbug_outs("\r\n");
	rtx_dbug_outs(&CharIn);
	rtx_dbug_outs("\r\n");
	switch(CharIn)
	{
	
		
			case '!':
				rtx_dbug_outs("! hotkey");
				print_ready_queue();
				break;
			case '@':
				rtx_dbug_outs("@ hotkey");
				print_blocked_memory_queue();
				break;
			case '#':
				rtx_dbug_outs("# hotkey");
				print_blocked_receive_queue();
				break;
			case '$':
				rtx_dbug_outs("$ hotkey");
				print_all_process_info();
				break;
			case '^':
				rtx_dbug_outs("^ hotkey");
				print_used_memory_block();
				break;			
			case '&':
				rtx_dbug_outs("& hotkey");
				print_message_queue();
				break;	
			default:
				rtx_dbug_outs("\r\n");
				rtx_dbug_outs("nothing found");
				break;
				

	}
	//TRACE ("Sending message to KCD \r\n");
	struct envelope *message_send = (struct envelope *)request_memory_block();
	message_send->message_data[0] = CharIn;
	//send_message(KCD_PROCESS_ID, message_send);
}


/**
 * @brief: Initializes the uart for serial IO 
 */
void uart_init()
{
	TRACE ("uart_init()\r\n");
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
}

/**
 * @brief: ISR for handling uart interrupts. Loads the uart I-process
 */
void c_uart_handler()
{
	/* Acknowledge interrupt by reading register */
	BYTE temp = SERIAL1_USR;

	//if (temp & 4)
	//	SERIAL1_IMR = 0x02;

	save_context(running_process->ID);
	
	if(running_process->state == STATE_RUNNING){
		running_process->state = STATE_READY;

		enqueue(ready_queue, 
				running_process->ID, 
				running_process->priority);

		running_process = NULL;
	}
	
	running_process = get_proc(UART_IPROCESS_ID);
	
	load_context(UART_IPROCESS_ID);
	return;
}

/**
 * @brief: Process responsible for performing uart IO operations
 */
void uart_iprocess()
{
	while(TRUE) 
	{
		TRACE("\n\n\n.......................uart_iprocess()\r\n");

		/* Acknowledge interrupt */
		BYTE temp = SERIAL1_USR;

		if (temp & 1)
		{
			hotkeys();
			TRACE("Receiving character...\r\n");

			/* Non-blocking call to request_memory_block() */
			void *mem_block = request_memory_block();
			if ( mem_block == NULL ) {
				rtx_dbug_outs(" Received NULL \r\n");
				} else {
				rtx_dbug_outs( " Msg Block = " );
				rtx_dbug_outs( itoa( mem_block ));
				rtx_dbug_outs( " \r\n " );
				}
				
			*(char *)(mem_block + 64) = SERIAL1_RD;
			send_message(KCD_PROCESS_ID, mem_block);
		}
		if (temp & 4)
		{
			TRACE("Transmitting character...\r\n");

			/* Non-blocking call to receive_mesasge() */
			struct envelope	*e = (struct envelope *)receive_message(NULL);
			
			//rtx_dbug_outs("Transmitting character2...\r\n");
			
			if (e != NULL)
			{
				//rtx_dbug_outs("Transmitting character3...\r\n");
				TRACE("e->message = ");
				TRACE((char *)e->message);
				TRACE("\r\n");
				SERIAL1_WD = *(char *)e->message;
				release_memory_block(e);
			}
		}

		release_processor();
	}
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

	/* Buffer to store keyboard input */
	char buffer[KCD_BUFFER_SIZE];
	int buffer_index = 0;

	while (TRUE)
	{
		rtx_dbug_outs("kcd_process()\r\n");

		struct envelope *e = (struct envelope *)receive_message(NULL);

		/* ----- COMMAND REGISTRATION ----- */
		if (e->sender_process_ID != UART_IPROCESS_ID)
		{
			/* Command registration message */
			/* Construct a new keyboard_command struct */
			struct keyboard_command *kc = 
				(struct keyboard_command *)malloc(sizeof(struct keyboard_command));

			/* Copy command identifier to struct */
			char *ch = (char *)e->message; 
			int i;
			for (i = 0; ch[i] != '\0' && i < MAX_IDENTIFIER_LENGTH; i++)
				kc->command_identifier[i] = ch[i];
			
			kc->command_identifier[i] = '\0';

			kc->registrant_process_ID = e->sender_process_ID;
			kc->next = NULL;

			/* Add to command registration list */
			if (head == NULL)
				head = kc;
			else
				tail->next = kc;

			tail = kc;
		}
		/* ----- KEYBOARD INPUT ----- */
		else
		{
			char *input_char = (char *)e->message;

			TRACE("Sending message to CRT display process\r\n");

			/* Send received character to CRT process to be echoed on screen */
			void *out_e = request_memory_block();
			TRACE("out_e = ");
			TRACE(itoa(out_e));
			TRACE("\r\n");
			*(char *)(out_e + 64) = *input_char;
			*(char *)(out_e + 65) = '\0';
			TRACE("out_e + 64 = ");
			TRACE((char *)(out_e + 64));
			rtx_dbug_outs("1\r\n");
			send_message(CRT_PROCESS_ID, out_e);
			rtx_dbug_outs("2\r\n");

			/* Add characters to buffer until user presses enter */
			if (*input_char == CR || *input_char == LF)
			{
				buffer[buffer_index] = '\0';
				buffer_index = 0;

				if (buffer[0] == COMMAND_PROMPT)
				{
					/* Extract command identifier from buffer */
					char ci[MAX_IDENTIFIER_LENGTH];
					int i = 0;
					while (buffer[i] != ' ')
					{
						ci[i] = buffer[i];
						i++;
					}
					ci[i] = '\0';

					/* Search registration list for identifier match */
					struct keyboard_command *kc = head;
					while (kc != NULL)
					{
						if (*kc->command_identifier == *ci)
						{
							TRACE("Match found! Sending to process ");
							TRACE(itoa(kc->registrant_process_ID));
							TRACE("\r\n");

							out_e = request_memory_block();
							*(char *)(out_e + 64) = *buffer;
							send_message(kc->registrant_process_ID, out_e);
							break;
						}
						kc = kc->next;
					}
				}
			}
			else if ( KCD_BUFFER_SIZE > buffer_index )
			
				buffer[buffer_index++] = *input_char;
		}

		release_memory_block((void *)e);
	}
}

/**
 * @brief: CRT display process responsible for displaying text to screen
 */
void crt_display_process()
{
	while(TRUE)
	{
		rtx_dbug_outs("crt_display_process()\n\r");
		
		struct envelope *msg_envl = (struct envelope *)receive_message(NULL);
		rtx_dbug_outs("In crt\n\r");

		TRACE("msg_envl = ");
		TRACE(itoa(msg_envl));
		TRACE("\r\n");

		/* Extract character(s) from message */
		char buffer[KCD_BUFFER_SIZE];
		int buffer_index = 0;

		while (*(char *)(msg_envl->message + buffer_index) != '\0')
		{
			buffer[buffer_index] = *(char *)(msg_envl->message + buffer_index);
			buffer_index++;
			rtx_dbug_outs("In here\n\r");
		}
		buffer[buffer_index] = '\0';
		buffer_index = 0;

		release_memory_block((void *)msg_envl);

		while(buffer[buffer_index] != '\0')
		{
			void *out_e = request_memory_block();
			*(char *)(out_e + 64) = buffer[buffer_index];
			send_message(UART_IPROCESS_ID, out_e);
			SERIAL1_IMR = 0x03;
			if (  buffer[buffer_index] == CR ) {
				void *out_e1 = request_memory_block();
				*(char *)(out_e1 + 64) = '\n';
				send_message(UART_IPROCESS_ID, out_e1);
				}
				
			buffer_index++;
		}
	}
}
