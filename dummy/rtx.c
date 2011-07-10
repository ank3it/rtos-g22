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
#include "util.h"


/* Interprocess Communications*/
int send_message (int process_ID, void * MessageEnvelope)
{
    TRACE("rtx: send_message \r\n");

	int result;
	//push d2 & d3 on stack
	asm("move.l %d2, -(%a7)");
	asm("move.l %d3, -(%a7)");
	
	asm("move.l %0, %%d2" : : "m" (process_ID));
	asm("move.l %0, %%d3 " : : "m" (MessageEnvelope));
	setpr(0);
	asm( "TRAP #10" );
	/* Get result out of d2 */
	asm("move.l %%d2, %0" : "=m" (result));
	//changing the values of registers back to their orginal values
	asm("move.l (%a7)+, %d3");
	asm("move.l (%a7)+, %d2");
	return result;
}

void * receive_message(int * sender_ID)
{
	void *result;
	//push d2 on stack
	asm("move.l %d2, -(%a7)");
	asm("move.l %d3, -(%a7)");
	
	asm("move.l %0, %%d2 " : : "m" (sender_ID));
	setpr(1);
	asm( "TRAP #10" );

	/* Get result from d2 */
	asm("move.l %%d2, %0" : "=m" (result));

	asm("move.l (%a7)+, %d3");
	asm("move.l (%a7)+, %d2");

	return result;
}

/*Memory Management*/
void * request_memory_block() 
{
	TRACE("rtx: request_memory_block \r\n");

	void *result;
	/* Save registers */
	asm("move.l %d2, -(%a7)");
    
	setpr(2);
	asm( "TRAP #10" );

	/* Get result out of d2 */
	asm("move.l %%d2, %0" : "=m" (result));

	/* Restore registers */
	asm("move.l (%a7)+, %d2");
    return result;
}

int release_memory_block(void * MemoryBlock)
{
    TRACE("rtx: release_memory_block \r\n");
	
	int result;
	/* Save registers */
	asm("move.l %d2, -(%a7)");

	asm("move.l %0, %%d2 " : : "m" (MemoryBlock));
	setpr(3);
	asm( "TRAP #10" );

	asm("move.l %%d2, %0" : "=m" (result));

	/* Restore registers */
	asm("move.l (%a7)+ , %d2");
    return result;
}

/*Process Management*/
int release_processor()
{
    TRACE("rtx: release_processor \r\n");

	int result;
	/* Save registers */
	asm("move.l %d2, -(%a7)");

	setpr(4);
	asm( "TRAP #10" );

	asm("move.l %%d2, %0" : "=m" (result));

	/* Restore registers */
	asm("move.l (%a7)+ , %d2");
    return result;
}

/*Timing Service*/
int delayed_send(int process_ID, void * MessageEnvelope, int delay)
{
    TRACE("................................................................rtx: delayed_send \r\n");

	int result;
	//push d2,d3 & d4 on stack
	asm("move.l %d2, -(%a7)");
	asm("move.l %d3, -(%a7)");
	asm("move.l %d4, -(%a7)");
	
    asm("move.l %0, %%d2 " : : "m" (process_ID));
	asm("move.l %0, %%d3 " : : "m" (MessageEnvelope));
	asm("move.l %0, %%d4 " : : "m" (delay));
	setpr(5);
	asm( "TRAP #10" );
	
	asm("move.l %%d2, %0" : "=m" (result));

	asm("move.l (%a7)+, %d4");
	asm("move.l (%a7)+, %d3");
	asm("move.l (%a7)+, %d2");
	return result;
}

/*Process Priority*/
int set_process_priority (int process_ID, int priority)
{
    TRACE("rtx: set_process_priority \r\n");

	int result;
	//push d2 & d3 on stack
	asm("move.l %d2, -(%a7)");
	asm("move.l %d3, -(%a7)");
	
    asm("move.l %0, %%d2 " : : "m" (process_ID));
	asm("move.l %0, %%d3 " : : "m" (priority));
	setpr(6);
	asm( "TRAP #10" );

	asm("move.l %%d2, %0" : "=m" (result));

	asm("move.l (%a7)+, %d3");
	asm("move.l (%a7)+, %d2");
	return result;
}

int get_process_priority (int process_ID)
{
    TRACE("rtx: get_process_priority \r\n");

	int result;
	//push d2 on stack
	asm("move.l %d2, -(%a7)");

    asm("move.l %0, %%d2 " : : "m" (process_ID));
	setpr(7);
	asm( "TRAP #10" );

	asm("move.l %%d2, %0" : "=m" (result));

	asm("move.l (%a7)+, %d2");
	return result;
}
