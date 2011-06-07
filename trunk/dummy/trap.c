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
			
			//changing the values of registers back to their orginal values
			asm("move.l (%a7)+, %d3");
			asm("move.l (%a7)+, %d2");
			
			break;
		
		case 1:
			asm("move.l %%d2 , %0" : "=m" (sender_ID));
			//k_receive_message(sender_ID);
			asm("move.l (%a7)+, %d2");
			break;
		
		case 2:
			//k_request_memory_block();
			break;
		
		case 3:
			asm("move.l %%d2 , %0" : "=m" (MemoryBlock));
			//k_release_memory_block(MemoryBlock);
			asm("move.l (%a7)+ , %d2");
			break;
			
		case 4:
			k_release_processor();
			break;
			
		case 5:
			asm("move.l %%d2 , %0" : "=m" (process_ID));
			asm("move.l %%d3 , %0" : "=m" (MessageEnvelope));
			asm("move.l %%d4 , %0" : "=m" (delay));
			//k_delayed_send(process_ID, MessageEnvelope, delay);
			
			asm("move.l (%a7)+, %d4");
			asm("move.l (%a7)+, %d3");
			asm("move.l (%a7)+, %d2");
			break;
			
		case 6:
			asm("move.l %%d2 , %0" : "=m" (process_ID));
			asm("move.l %%d3 , %0" : "=m" (priority));
			k_set_process_priority(process_ID,priority);
			
			asm("move.l (%a7)+, %d3");
			asm("move.l (%a7)+, %d2");
			break;
		
		case 7:
			asm("move.l %%d2 , %0" : "=m" (process_ID));
			
			k_get_process_priority(process_ID);
			
			asm("move.l (%a7)+, %d2");
			break;
		
		default:
			break;
	}

	asm("rte");

}

/*
 * gcc expects this function to exist
 */
int init_trap( void )
{
    /* Load the vector table for TRAP #10 with our assembly stub
       address */
    asm( "move.l #c_trap_handler,%d0" );
    asm( "move.l %d0,0x100000A8" );

	rtx_dbug_outs( (CHAR *) "in Trap Init!!\n\r" );

    return 0;
}