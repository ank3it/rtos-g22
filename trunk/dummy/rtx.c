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
#include "../trap/trap.h"


/* Interprocess Communications*/
int send_message (int process_ID, void * MessageEnvelope)
{
    rtx_dbug_outs((CHAR *)"rtx: send_message \r\n");
	//push d2 & d3 on stack
	asm("move.l %d2, -(%a7)");
	asm("move.l %d3, -(%a7)");
	
	asm("move.l process_ID,%d2 ");
	asm("move.l MessageEnvelope,%d3 ");
	setpr(0);
	asm( "TRAP #10" );
	return 0;
}

void * receive_message(int * sender_ID)
{
	//push d2 on stack
	asm("move.l %d2, -(%a7)");
	
    rtx_dbug_outs((CHAR *)"rtx: receive_message \r\n");
	asm("move.l sender_ID,%d2 ");
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
	
	asm("move.l MemoryBlock,%d2 ");
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
    asm("move.l process_ID,%d2 ");
	asm("move.l MessageEnvelope,%d3 "); 
	asm("move.l delay,%d4 ");
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
    asm("move.l process_ID,%d2 ");
	asm("move.l priority,%d3 ");
	setpr(6);
	asm( "TRAP #10" );
	return 0;
}

int get_process_priority (int process_ID)
{
	//push d2 on stack
	asm("move.l %d2, -(%a7)");

    rtx_dbug_outs((CHAR *)"rtx: get_process_priority \r\n");
	asm("move.l process_ID,%d2 ");
	setpr(7);
	asm( "TRAP #10" );
	return 0;
}
