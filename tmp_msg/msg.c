/**
 * @group: S11-G22
 * @file: msg.c
 * @date: 2011/06/24
 * @brief: Contains functions for sending and receiving messages
 */

#include "msg.h"

#include "../process/process.c"

struct process * dest_process;

int send_message(int process_ID, void * MessageEnvelope)
{
	dest_process = get_proc(MessageEnvelope->dest_proc_id);

	
	if( dest_process->state == STATE_BLOCKED){
		
		//enqueue(blocked_queue, dest_process->ID, dest_process->priority);
	
	
	}else{
	
	}



}