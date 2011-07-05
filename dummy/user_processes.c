#include "user_processes.h"
#include "envelope.h"
#include "memory.h"
#include "malloc.h"

//added a temporary message queue
void enqueue_local_message(envelope *message)
{
	struct temp_msg_queue *new_node = malloc(sizeof(struct temp_msg_queue));
	new_node->value = message;

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
	p = request_memory_block();
	//not sure about the commands yet
	command('Z');
	
	while(1)
	{
		p = (envelope *)receive_message(NULL);
		if(message p contains 'Z')
		{
			release_memory_block((void *)p);
			break;
		}
		else
			release_memory_block((void *)p);
	}
	
	num = 0;
	while(1)
	{
		p = (envelope *)request_memory_block();
		p->message_type = COUNT_REPORT;
		p->data = num;
		send_message(PROCESS_B, p);
		num = num + 1;
		release_processor();
	}
}

void ProcessB()
{
	struct envelope *pB;
	while(1)
	{
		pB = receive_message(NULL);
		send_message(PROCESS_C, pB);
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
			pC = (envelope *)receive_message(NULL);
		}
		else
		{
			pC = temp_message_head;
			temp_message_head = temp_message_head->next;
		}
		
		if(p->message_type == COUNT_REPORT)
		{
			if(msg_data[0] % 20 == 0)
			{
				pC = (envelope *)request_memory_block();
				send_message(CRT, pC);
				
				q = (envelope *)request_memory_block();
				q->message_type = WAKEUP10;
				//not sure
				delayed_send(PROCESS_C_ID ,q, 10000);
				
				while(1)
				{
					pC = (envelope *)receive_message(NULL);
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