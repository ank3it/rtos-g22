/**
 * @file: trap.c
 * @brief: software interuupt (SWI) sample code
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang
 * @date: 2011/01/04
 */

#include "rtx_inc.h"
#include "dbug.h"
//#include "process.h"

extern void k_release_processor();

int CURR_TRAP = 0;
void setpr(int value) {
	CURR_TRAP = value;
}

/*
 * This function is called by the assembly STUB function
 */
VOID c_trap_handler( VOID )
{
    int process_ID, delay, priority;
	void * MessageEnvelope, * MemoryBlock;
	int * sender_ID;
	
	rtx_dbug_outs( (CHAR *) "Trap Handler!!\n\r" );
	switch (CURR_TRAP) {
		case 0:
			//initializing the variables
			asm("move.l %%d2 , %0" : "=m" (process_ID));
			asm("move.l %%d3 , %0" : "=m" (MessageEnvelope));
			
			//k_send_message(process_ID,MessageEnvelope);
			
			break;
		
		case 1:
			asm("move.l %%d2 , %0" : "=m" (sender_ID));
			//k_receive_message(sender_ID);
			
			break;
		
		case 2:
			//k_request_memory_block();
			break;
		
		case 3:
			asm("move.l %%d2 , %0" : "=m" (MemoryBlock));
			//k_release_memory_block(MemoryBlock);
			
			break;
			
		case 4:
			k_release_processor();
			
			break;
			
		case 5:
			asm("move.l %%d2 , %0" : "=m" (process_ID));
			asm("move.l %%d3 , %0" : "=m" (MessageEnvelope));
			asm("move.l %%d4 , %0" : "=m" (delay));
			//k_delayed_send(process_ID, MessageEnvelope, delay);
			
			break;
			
		case 6:
			asm("move.l %%d2 , %0" : "=m" (process_ID));
			asm("move.l %%d3 , %0" : "=m" (priority));
			k_set_process_priority(process_ID,priority);
			
			break;
		
		case 7:
			asm("move.l %%d2 , %0" : "=m" (process_ID));
			
			int result = k_get_process_priority(process_ID);
			rtx_dbug_outs(itoa(result));
			
			asm("move.l %0, %%d2" : : "m" (result));

			break;
		
		default:
			break;
	}
}

/*
 * gcc expects this function to exist
 */
int init_trap( void )
{
    /* Load the vector table for TRAP #10 with our assembly stub
       address */
    asm( "move.l #asm_trap_entry,%d0" );
    asm( "move.l %d0,0x100000A8" );

	rtx_dbug_outs( (CHAR *) "in Trap Init!!\n\r" );

    return 0;
}
