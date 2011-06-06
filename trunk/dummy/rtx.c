/*--------------------------------------------------------------------------
 *                      RTX Stub 
 *--------------------------------------------------------------------------
 */
/**
 * @file:   rtx.c   
 * @author: Irene Huang
 * @date:   2010.05.15
 * @brief:  Dummy rtx primitive stub implementation 
 */

#include "rtx_inc.h"
#include "rtx.h"
#include "dbug.h"
#include "trap.h"


/* Interprocess Communications*/
int send_message (int process_ID, void * MessageEnvelope)
{
    rtx_dbug_outs((CHAR *)"rtx: send_message \r\n");
	//push d2 & d3 on stack
	asm("move.l %d2, -(%a7)");
	asm("move.l %d3, -(%a7)");
	
	asm("move.l %0, %%d2" : : "m" (process_ID));
	asm("move.l %0, %%d3 " : : "m" (MessageEnvelope));
	setpr(0);
	asm( "TRAP #10" );
	return 0;
}

void * receive_message(int * sender_ID)
{
	//push d2 on stack
	asm("move.l %d2, -(%a7)");
	
    rtx_dbug_outs((CHAR *)"rtx: receive_message \r\n");
	asm("move.l %0, %%d2 " : : "m" (sender_ID));
	setpr(1);
	asm( "TRAP #10" );
	
}

/*Memory Management*/
void * request_memory_block() 
{
    rtx_dbug_outs((CHAR *)"rtx: request_memory_block \r\n");
	setpr(2);
	asm( "TRAP #10" );
    return NULL;
}

int release_memory_block(void * MemoryBlock)
{
    rtx_dbug_outs((CHAR *)"rtx: release_memory_block \r\n");
	
	//push d2 on stack
	asm("move.l %d2, -(%a7)");
	asm("move.l %0, %%d2 " : : "m" (MemoryBlock));
	setpr(3);
	asm( "TRAP #10" );
    return 0;
}

/*Process Management*/
int release_processor()
{
    rtx_dbug_outs((CHAR *)"rtx: release_processor \r\n");
	setpr(4);
	asm( "TRAP #10" );
    return 0;
}

/*Timing Service*/
int delayed_send(int process_ID, void * MessageEnvelope, int delay)
{
	//push d2,d3 & d4 on stack
	asm("move.l %d2, -(%a7)");
	asm("move.l %d3, -(%a7)");
	asm("move.l %d4, -(%a7)");
	
	
    rtx_dbug_outs((CHAR *)"rtx: delayed_send \r\n");
    asm("move.l %0, %%d2 " : : "m" (process_ID));
	asm("move.l %0, %%d3 " : : "m" (MessageEnvelope));
	asm("move.l %0, %%d4 " : : "m" (delay));
	setpr(5);
	asm( "TRAP #10" );
	return 0;
}

/*Process Priority*/
int set_process_priority (int process_ID, int priority)
{
	//push d2 & d3 on stack
	asm("move.l %d2, -(%a7)");
	asm("move.l %d3, -(%a7)");
	
    rtx_dbug_outs((CHAR *)"rtx: set_process_priority \r\n");
    asm("move.l %0, %%d2 " : : "m" (process_ID));
	asm("move.l %0, %%d3 " : : "m" (priority));
	setpr(6);
	asm( "TRAP #10" );
	return 0;
}

int get_process_priority (int process_ID)
{
	//push d2 on stack
	asm("move.l %d2, -(%a7)");

    rtx_dbug_outs((CHAR *)"rtx: get_process_priority \r\n");
    asm("move.l %0, %%d2 " : : "m" (process_ID));
	setpr(7);
	asm( "TRAP #10" );
	return 0;
}
