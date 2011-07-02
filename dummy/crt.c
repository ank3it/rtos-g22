/**
 * @group: S11-G22
 * @file: crt.h
 * @date: 2011/06/22
 * @brief: Contains CRT function that contains character string 
		   that is to be displayed
 */

#include "crt.h"

void crt()
{ 
	while(1)
	{		
		TRACE("Entering CRT....\n\r");
		
		envelope * msg_envl = (envelope *)receive_message(NULL);
		TRACE("Message envelope = ");
		TRACE(itoa(msg_envl));
		TRACE("\r\n");
		
		int i = 0;
		if(msg_envl->dest_process_ID != CRT_ID)
		{
			TRACE("You are not in the CRT process");
		}
		else
		{
			while(msg_envl->message_data[i] != "\0")
			{
				envelope * UART_msg_envl = (envelope *)request_memory_block();
				UART_msg_envl->message_data[0] = msg_envl->message_data[i];
				send_message(UART_ID, UART_msg_envl);
				i++;
			}
		}
		release_memory_block((void *) msg_envl);		
		}
	}
}
