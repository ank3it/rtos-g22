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
	//p = request_memory_block();

	//z-command declaration 
	
	void *mem_block2 = request_memory_block();
	*(char *)(mem_block2 + 64) = 'Z';
	*(char *)(mem_block2 + 65) = '\0';	
	send_message(KCD_PROCESS_ID , mem_block2);
	
	int sender_ID = -1;
	

	///
	//not sure about the commands yet
	//command('Z');
	
	while(TRUE)
	{
		struct envelope *envelope  = receive_message(&sender_ID);
		
		////rtx_dbug_outs("In process A. Sender ID is: ");
		////rtx_dbug_outs(itoa(sender_ID));
		//rtx_dbug_outs("\r\n");
		if ( envelope->message != NULL ) {
			
			//rtx_dbug_outs("Got message \r\n");
			/* Extract character(s) from message */
			char buffer[KCD_BUFFER_SIZE];
			int buffer_index = 0;
			
			while (*(char *)(envelope->message + buffer_index) != '\0')
			{
				buffer[buffer_index] = *(char *)(envelope->message + buffer_index);
				buffer_index++;
			}
			
			buffer[buffer_index] = '\0';
			buffer_index = 0;
			//rtx_dbug_outs(buffer);
		
			if(buffer[1]== 'Z')
			{				
				release_memory_block((void *)envelope);
				//rtx_dbug_outs("Process A's exting ");
				//rtx_dbug_outs(itoa(sender_ID));
				//rtx_dbug_outs("\r\n");
				break;
			}
			else{
				release_memory_block((void *)envelope);
				//rtx_dbug_outs("Process A's  ");
			}
		}			
	}
	
	int num = 0;
	struct envelope *envelope;
	while(TRUE)
	{
		envelope = (struct envelope *)request_memory_block();
		envelope->message_type = COUNT_REPORT;
		////rtx_dbug_outs("Process A's exting 3");
		*(int *)((((char *)(envelope))) + 64 ) = num;
		////rtx_dbug_outs("Process A's exting 2");
		//*(int *)envelope->message = num;		
		send_message(PROCESS_B_ID, envelope);
		num = num + 1;
		////rtx_dbug_outs(itoa(num));
		release_processor();
	}
}

void ProcessB()
{
	struct envelope *pB;
	while(1)
	{
		////rtx_dbug_outs ( " In process B \r\n");
		pB = (struct envelope *) receive_message(NULL);
		send_message(PROCESS_C_ID, pB);
		//release_memory_block((void *)pB);
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
			pC = temp_message_head->message;
			//free(temp_message_head);
			temp_message_head = temp_message_head->next;
		}
		if(pC->message_type == COUNT_REPORT)
		{
			//rtx_dbug_outs ( itoa(*(int *)((((char *)(pC))) + 64 )));
			//rtx_dbug_outs ( " In process C5 \r\n");
			if(*(int *)((((char *)(pC))) + 64 )% 20 == 0)
			{
				////rtx_dbug_outs ( " In process C6 \r\n");
						char timeStr[] = "Process C"; 
						int i = 0;
						//void *pC1 = request_memory_block();
						for ( i = 0 ; i < 12 ; i++ ) {
							*((((char *)(pC))) + i + 64 ) = timeStr[i];
						}

				//pC->message = "Process C";
				send_message(CRT_PROCESS_ID, pC);
				
				// HIBERNATE START
				q = (struct envelope *)request_memory_block();
				q->message_type = WAKEUP10;
				//not sure
				delayed_send(PROCESS_C_ID ,q, 10000);
				
				while(1)
				{
					pC = (struct envelope *)receive_message(NULL);
					if(pC->message_type == WAKEUP10)
					{
						////rtx_dbug_outs ( " IN WAKE UP 10 \r\n");
						break;
					}
					else
					{
						enqueue_local_message(pC);
					}
				}

				// HIBERNATE END	
			}
		}		
		release_memory_block((void *)pC);
		release_processor();
	}
}