/**
 * @group: S11-G22
 * @file: sppc.c
 * @brief: Setting up priority process command
 * @date: 2011/07/04
 */
 
#include "sppc.h"

void set_priority_process()
{
	int process_id = 0;
	int priority = 0;
	envelope * msg_envl = NULL;
	msg_envl->message_data[0] = '%';
	msg_envl->message_data[1] = 'C';
	msg_envl = (envelope *)k_request_memory_block();
	
	while(1)
	{
		msg_envl = (envelope *)k_receive_message(NULL);
		if(msg_envl->message_data[2] = ' ')
		{
			process_id = atoi(message_data[3]);
			priority = atoi(message_data[5]);
			if(process_id >= 0 && priority >= 0 && priority < 4)
			{
				TRACE("Inputed values are valid");
				TRACE("\r\n");
				k_set_process_priority(process_id, priority);
			}
			else
			{
					UINT32 i = 3;
					while(msg_envelope->message_data[i] != NULL)
					{
						if(msg_envelope->message_data[i] == ' ')
							break;
						i++;
					}
					
					Trace("Value(s) inputted are incorrect");
					TRACE("\r\n");
					k_send_message(CRT_ID, msg_envl);
			}
			k_release_memory_block(msg_envl);
		}
	}
}