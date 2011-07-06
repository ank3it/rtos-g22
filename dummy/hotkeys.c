/**
 * @group: S11-G22
 * @file: hotkeys.c
 * @brief: Initializing hotkeys
 * @date: 2011/07/02
 */
 
#include "hotkeys.h"

void print_ready_queue()
{
	struct process *proc;
	proc = ready_queue->head;
	
	TRACE("\r\n");
	while(proc != ready_queue->tail)
	{
		TRACE("Process ID: ");
		TRACE(itoa(proc->ID));
		TRACE("\r\n");
		TRACE("Process Priority: ");
		TRACE(itoa(proc->priority));
		TRACE("\r\n");
		proc = ready_queue->head->next;
	}
}

void print_blocked_memory_queue()
{
	
	struct process *proc;
	proc = blocked_queue->head;
	
	while(proc != blocked_queue->tail)
	{
		TRACE("\r\n");
		if(proc->block_type == BLOCK_MEMORY)
		{
			TRACE("Process ID: ");
			TRACE(itoa(proc->ID));
			TRACE("\r\n");
			TRACE("Process Priority: ");
			TRACE(itoa(proc->priority));
			TRACE("\r\n");
			proc = blocked_queue->head->next;
		}
	}
}

void print_blocked_receive_queue()
{
	int block_type;
	struct process *proc;
	proc = blocked_queue->head;
	
	while(proc != blocked_queue->tail)
	{
		TRACE("\r\n");
		if(proc->block_type == = BLOCK_RECEIVE)
		{
			TRACE("Process ID: ");
			TRACE(itoa(proc->ID));
			TRACE("\r\n");
			TRACE("Process Priority: ");
			TRACE(itoa(proc->priority));
			TRACE("\r\n");
			proc = blocked_queue->head->next;
		}
	}
}

//TODO: Print available memory and used memory
void print_used_memory_block()
{
	int traverse_mem;
	int chk_mem_blk;
	TRACE("Printing out used memory blocks\r\n");
	while ( i< NUM_MEM_BLKS ) {
		traverse_mem = (int)head + ( NUM_MEM_BLKS + 1 )*4 + ( MAX_BLK_SIZE / 4 * i )*4; 
		if ( chk_mem_blk == traverse_mem ) {
			TRACE(itoa(traverse_mem));
			TRACE("\r\n");
			break;
		}
		i++;
	}
}

void print_all_process_info()
{
	TRACE("Printing all process queues");
	TRACE("\r\n");
	TRACE("Printing ready queue:");
	TRACE("\r\n");
	print_ready_queue;
	TRACE("\r\n");
	TRACE("Printing blocked on memory queue:");
	TRACE("\r\n");
	print_blocked_memory_queue;
	TRACE("\r\n");
	TRACE("Printing blocked on receive queue:");
	TRACE("\r\n");
	print_blocked_receive_queue;	
}

void print_message_queue()
{
	struct process *proc;
	struct envelope *e = proc->mailbox_head;
	
	TRACE("\r\n");
	while(e != proc->mailbox_tail)
	{
		TRACE("Sender Process ID: ");
		TRACE(itoa(e->sender_process_ID));
		TRACE("\r\n");
		TRACE("Destination Process ID: ");
		TRACE(itoa(e->dest_process_ID));
		TRACE("\r\n");
		TRACE(itoa(e->message_type));
		TRACE("\r\n");
		e = e->next;
	}
}