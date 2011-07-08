/**
 * @file: trap.c
 * @brief: software interuupt (SWI) sample code
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang
 * @date: 2011/01/04
 */

#include "rtx_inc.h"
#include "defs.h"
#include "dbug.h"
#include "process.h"
#include "memory.h"
#include "util.h"
#include "trap.h"

int CURR_TRAP = 0;
void setpr(int value) {
	CURR_TRAP = value;
}

/*
 * This function is called by the assembly STUB function
 */
void c_trap_handler()
{
	TRACE("c_trap_handler()\r\n");

	execute_sys_call();

	/* If this process was previously blocked then it has a pending
	 * system call that needs to be completed before returning to
	 * the user process */
	if (running_process->pending_sys_call != SYS_CALL_NONE)
	{
		CURR_TRAP = running_process->pending_sys_call;
		execute_sys_call();
		running_process->pending_sys_call = SYS_CALL_NONE;
	}
}

void execute_sys_call()
{
	TRACE("execute_sys_call()\r\n");

    int process_ID, delay, priority;
	void * MessageEnvelope, * MemoryBlock;
	int * sender_ID;
	int result1;
	void *result2 = NULL;
	switch (CURR_TRAP) {
		case SYS_CALL_SEND_MESSAGE:
			//initializing the variables
			asm("move.l %%d2 , %0" : "=m" (process_ID));
			asm("move.l %%d3 , %0" : "=m" (MessageEnvelope));

			result1 = k_send_message(process_ID,MessageEnvelope);
			
			asm("move.l %0, %%d2" : : "m" (result1));
			break;
		
		case SYS_CALL_RECEIVE_MESSAGE:
			asm("move.l %%d2 , %0" : "=m" (sender_ID));
			result2 = k_receive_message(sender_ID);
			
			asm("move.l %0, %%d2" : : "m" (result2));
			asm("move.l %0, %%d3" : : "m" (sender_ID));
			break;
		
		case SYS_CALL_REQUEST_MEMORY_BLOCK:
			result2 = k_request_memory_block();

			asm("move.l %0, %%d2" : : "m" (result2));
			break;
		
		case SYS_CALL_RELEASE_MEMORY_BLOCK:
			asm("move.l %%d2 , %0" : "=m" (MemoryBlock));
			result1 = k_release_memory_block(MemoryBlock);

			asm("move.l %0, %%d2" : : "m" (result1));
			break;
			
		case SYS_CALL_RELEASE_PROCESSOR:
			result1 = k_release_processor();
			
			asm("move.l %0, %%d2" : : "m" (result1));
			break;
			
		case SYS_CALL_DELAYED_SEND:
			asm("move.l %%d2 , %0" : "=m" (process_ID));
			asm("move.l %%d3 , %0" : "=m" (MessageEnvelope));
			asm("move.l %%d4 , %0" : "=m" (delay));
			k_send_delay(process_ID, MessageEnvelope, delay);

			asm("move.l %0, %%d2" : : "m" (result1));
			break;
			
		case SYS_CALL_SET_PROCESS_PRIORITY:
			asm("move.l %%d2 , %0" : "=m" (process_ID));
			asm("move.l %%d3 , %0" : "=m" (priority));
			result1 = k_set_process_priority(process_ID,priority);
			
			asm("move.l %0, %%d2" : : "m" (result1));
			break;
		
		case SYS_CALL_GET_PROCESS_PRIORITY:
			asm("move.l %%d2 , %0" : "=m" (process_ID));
			
			result1 = k_get_process_priority(process_ID);
			TRACE(itoa(result1));
			
			asm("move.l %0, %%d2" : : "m" (result1));
			break;
		
		default:
			break;
	}

	scheduler_run();
}

/*
 * gcc expects this function to exist
 */
int init_trap( void )
{
	TRACE("init_trap()\r\n");
    /* Load the vector table for TRAP #10 with our assembly stub
       address */
    asm( "move.l #asm_trap_entry,%d0" );
    asm( "move.l %d0,0x100000A8" );

    return 0;
}
