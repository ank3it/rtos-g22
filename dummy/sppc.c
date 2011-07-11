/**
 * @group: S11-G22
 * @file: sppc.c
 * @brief: Setting up priority process command
 * @date: 2011/07/04
 */
 
#include "sppc.h"

void set_priority_process()
{
	void *mem_block2 = request_memory_block();
    *(char *)(mem_block2 + 64) = 'C';
    *(char *)(mem_block2 + 65) = '\0';  
	send_message(KCD_PROCESS_ID , mem_block2);
	int process_id = 0;
	int priority = 0;
//	struct envelope * msg_envl = NULL;
	//msg_envl->message_data[0] = '%';
	//msg_envl->message_data[1] = 'C';
	//msg_envl = (struct envelope *)request_memory_block();
	
	while(TRUE)
	{
		int sender_ID = -1;
		struct envelope * msg_envl = receive_message(&sender_ID);
		
		 rtx_dbug_outs(" In KCD \r\n");
		 rtx_dbug_outs(itoa(sender_ID));
		
		if(msg_envl->message != NULL)
		{
			rtx_dbug_outs(" In KCD \r\n");
			char buffer[KCD_BUFFER_SIZE];
			int buffer_index = 0;
			
			while (*(char *)(msg_envl->message + buffer_index) != '\0')
            {
                buffer[buffer_index] = *(char *)(msg_envl->message + buffer_index);
                //rtx_dbug_outs(buffer[buffer_index]);
                buffer_index++;                
            }
			buffer[buffer_index] = '\0';
			buffer_index = 0;
			rtx_dbug_outs(" Buffer = ");
			rtx_dbug_outs(buffer);
			

			if(buffer[2] = ' ')
			{
				process_id = atoi(&buffer[3]);
				rtx_dbug_outs(itoa(process_id));
				priority = atoi(&buffer[5]);
				rtx_dbug_outs(itoa(priority));
				rtx_dbug_outs(" \r\n");
				if(process_id >= 0 && priority >= 0 && priority < 4)
				{
					rtx_dbug_outs("Inputed values are valid");
					rtx_dbug_outs("\r\n");
					set_process_priority(process_id, priority);
				}
				else
				{
						if(buffer[3] = ' ')
						{
							rtx_dbug_outs("No value inputted\r\n");
						}
						UINT32 i = 3;
						while(buffer[i] != NULL)
						{
							if(buffer[i] == ' ')
								break;
							i++;
						}
						
						rtx_dbug_outs("Value(s) inputted are incorrect");
						rtx_dbug_outs("\r\n");
						send_message(CRT_PROCESS_ID, msg_envl);
				}
				release_memory_block(msg_envl);
			}
		}
	}
}