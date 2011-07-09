/**
 * @group: S11-G22
 * @file: hotkeys.c
 * @brief: Initializing hotkeys
 * @date: 2011/07/02
 */
 
#include "hotkeys.h"

void print_ready_queue()
{
	rtx_dbug_outs_hotkeys("\r\n");
	rtx_dbug_outs_hotkeys("Print ready queue");
	struct queue_node *proc;
	proc = ready_queue->head;
	
	rtx_dbug_outs_hotkeys("\r\n");
	
	while(proc != NULL)
	{
		rtx_dbug_outs_hotkeys("Process ID: ");
		rtx_dbug_outs_hotkeys(itoa(proc->value));
		rtx_dbug_outs_hotkeys("\r\n");
		rtx_dbug_outs_hotkeys("Process Priority: ");
		rtx_dbug_outs_hotkeys(itoa(proc->priority));
		rtx_dbug_outs_hotkeys("\r\n");
		proc = proc->next;
	}
}

void print_blocked_memory_queue()
{	
	rtx_dbug_outs_hotkeys("\r\n");
	rtx_dbug_outs_hotkeys("Print blocked memory queue");
	//rtx_dbug_outs_hotkeys("\r\n");
	
	struct queue_node *proc;
	proc = blocked_queue->head;
	int block_type = (get_proc(proc->value))->block_type;
	//rtx_dbug_outs_hotkeys(itoa(block_type));
	
	while(proc != NULL)
	{
		rtx_dbug_outs_hotkeys("\r\n");
		if(block_type == BLOCK_MEMORY)
		{
			rtx_dbug_outs_hotkeys("Process ID: ");
			rtx_dbug_outs_hotkeys(itoa(proc->value));
			rtx_dbug_outs_hotkeys("\r\n");
			rtx_dbug_outs_hotkeys("Process Priority: ");
			rtx_dbug_outs_hotkeys(itoa(proc->priority));
			rtx_dbug_outs_hotkeys("\r\n");			
		}
		proc = proc->next;
	}
}

void print_blocked_receive_queue()
{
	rtx_dbug_outs_hotkeys("\r\n");
	rtx_dbug_outs_hotkeys("Print blocked receive queue");

	struct queue_node *proc;
	proc = blocked_queue->head;
	int block_type = get_proc(proc->value)->block_type;
	
	while(proc != blocked_queue->tail)
	{
		rtx_dbug_outs_hotkeys("\r\n");
		if(block_type == BLOCK_RECEIVE)
		{
			rtx_dbug_outs_hotkeys("Process ID: ");
			rtx_dbug_outs_hotkeys(itoa(proc->value));
			rtx_dbug_outs_hotkeys("\r\n");
			rtx_dbug_outs_hotkeys("Process Priority: ");
			rtx_dbug_outs_hotkeys(itoa(proc->priority));
			rtx_dbug_outs_hotkeys("\r\n");			
		}
		proc = proc->next;
	}
}

//TODO: Print used memory
void print_used_memory_block()
{
	rtx_dbug_outs_hotkeys("\r\n");

	//void *memory_block;
	int *head;
	int traverse_mem;
	//int chk_mem_blk = memory_block;
	int i = 0;
	rtx_dbug_outs_hotkeys("Printing out used memory blocks\r\n");
	while ( i< NUM_MEM_BLKS ) {
		traverse_mem = (int)head + ( NUM_MEM_BLKS + 1 )*4 + ( MAX_BLK_SIZE / 4 * i )*4; 
		rtx_dbug_outs_hotkeys("\r\n");
		rtx_dbug_outs_hotkeys("Print out traverse_mem = ");
		rtx_dbug_outs_hotkeys(itoa(traverse_mem));
		
		// if ( chk_mem_blk == traverse_mem ) {
			// rtx_dbug_outs_hotkeys("Used memory block = ");
			// rtx_dbug_outs_hotkeys(itoa(traverse_mem));
			// rtx_dbug_outs_hotkeys("\r\n");
			// break;
		// }
		i++;
		
	}
	rtx_dbug_outs_hotkeys("\r\n");
	rtx_dbug_outs_hotkeys(itoa(i));
	rtx_dbug_outs_hotkeys("\r\n");
}

void print_all_process_info()
{
	rtx_dbug_outs_hotkeys("\r\n");
	rtx_dbug_outs_hotkeys("Print all process info");
	rtx_dbug_outs_hotkeys("\r\n");

	rtx_dbug_outs_hotkeys("Printing all process queues");
	rtx_dbug_outs_hotkeys("\r\n");
	rtx_dbug_outs_hotkeys("Printing ready queue:");
	rtx_dbug_outs_hotkeys("\r\n");
	print_ready_queue();
	rtx_dbug_outs_hotkeys("\r\n");
	rtx_dbug_outs_hotkeys("Printing blocked on memory queue:");
	rtx_dbug_outs_hotkeys("\r\n");
	print_blocked_memory_queue();
	rtx_dbug_outs_hotkeys("\r\n");
	rtx_dbug_outs_hotkeys("Printing blocked on receive queue:");
	rtx_dbug_outs_hotkeys("\r\n");
	print_blocked_receive_queue();	
}

void print_message_queue()
{
	rtx_dbug_outs_hotkeys("\r\n");
	rtx_dbug_outs_hotkeys("Print message queues");
	
	struct process *proc;
	struct envelope *e = proc->mailbox_head;
	
	
	while(e != NULL)
	{
		rtx_dbug_outs_hotkeys("\r\n");
		rtx_dbug_outs_hotkeys("Sender Process ID: ");
		rtx_dbug_outs_hotkeys(itoa(e->sender_process_ID));
		rtx_dbug_outs_hotkeys("\r\n");
		rtx_dbug_outs_hotkeys("Destination Process ID: ");
		rtx_dbug_outs_hotkeys(itoa(e->dest_process_ID));
		rtx_dbug_outs_hotkeys("\r\n");
		rtx_dbug_outs_hotkeys("Message type: ");
		rtx_dbug_outs_hotkeys(itoa(e->message_type));
		rtx_dbug_outs_hotkeys("\r\n");
		e = e->next;
	}
}