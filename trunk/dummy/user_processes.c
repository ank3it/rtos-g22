#include "user_processes.h"
#include "envelope.h"
#include "memory.h"
#include "malloc.h"
#include "defs.h"
#include "process.h"

//added a temporary message queue
void enqueue_local_message(struct envelope *message)
{
	struct temp_msg_queue *new_node = malloc(sizeof(struct temp_msg_queue));
	new_node->message = message;

	if(temp_message_head == NULL)
	{
		temp_message_head = temp_message_tail = new_node;
		new_node-> next = NULL;		
	}
	else
	{
		temp_message_tail = new_node;
		temp_message_tail = new_node;
	}
}

void ProcessA()
{
	struct envelope *p;
	p = k_request_memory_block();

	//z-command declaration 
	
	void *mem_block2 = request_memory_block();
	*(char *)(mem_block2 + 64) = 'Z';
	*(char *)(mem_block2 + 65) = '\0';	
	send_message(KCD_PROCESS_ID , mem_block2);

	///
	//not sure about the commands yet
	//command('Z');
	
	while(TRUE)
	{
		
		int sender_ID = -1;
		
		struct envelope *envelope  = receive_message(&sender_ID);
		
		rtx_dbug_outs(" In KCD \r\n");
		rtx_dbug_outs(itoa(sender_ID));
		if ( envelope->message != NULL ) {
			
			rtx_dbug_outs(" In KCD \r\n");
			/* Extract character(s) from message */
			char buffer[KCD_BUFFER_SIZE];
			int buffer_index = 0;
			
			while (*(char *)(envelope->message + buffer_index) != '\0')
			{
				buffer[buffer_index] = *(char *)(envelope->message + buffer_index);
				rtx_dbug_outs(buffer[buffer_index]);
				buffer_index++;
			}
			
			buffer[buffer_index] = '\0';
			buffer_index = 0;
			rtx_dbug_outs(buffer);
		
			p = (struct envelope *)receive_message(NULL);
			
			if(buffer[2]== 'Z')
			{
				release_memory_block((void *)p);
				break;
			}
			else{
				release_memory_block((void *)p);
			}
		}
	}
	
	int num = 0;
	while(1)
	{
		p = (struct envelope *)request_memory_block();
		p->message_type = COUNT_REPORT;
		*(int *)p->message = num;		
		send_message(PROCESS_B_ID, p);
		num = num + 1;
		release_processor();
	}
}

void ProcessB()
{
	struct envelope *pB;
	while(1)
	{
		pB = (struct envelope *) receive_message(NULL);
		send_message(PROCESS_C_ID, pB);
		release_memory_block((void *)pB);
	}
}

void ProcessC()
{
	temp_message_head = NULL;
	temp_message_tail = NULL;
	struct envelope *pC;
	struct envelope *q;
	
	while(1)
	{
		if(temp_message_head == NULL)
		{
			pC = (struct envelope *)receive_message(NULL);
		}
		else
		{
			pC = temp_message_head;
			temp_message_head = temp_message_head->next;
		}
		
		if(pC->message_type == COUNT_REPORT)
		{
			if(*(int *)(pC->message) % 20 == 0)
			{
				pC = (struct envelope *)request_memory_block();
				send_message(CRT_PROCESS_ID, pC);
				
				q = (struct envelope *)request_memory_block();
				q->message_type = WAKEUP10;
				//not sure
				delayed_send(PROCESS_C_ID ,q, 10000);
				
				while(1)
				{
					pC = (struct envelope *)receive_message(NULL);
					if(pC->message_type = WAKEUP10)
					{
						break;
					}
					else
					{
						enqueue_local_message(pC);
					}
				}				
			}
		}		
		release_memory_block((void *)pC);
		release_processor();
	}
}