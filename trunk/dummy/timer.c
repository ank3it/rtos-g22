/**
 * @file: timer.c
 * @brief: timer 0 smaple code
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang
 * @date: 2011/01/04
 */


#include "rtx_inc.h"
//#include "uart1.h"
#include "timer.h"
#include "dbug.h"
#include "util.h"
#include "queue.h"
#include "defs.h"
#include "process.h"

#define MaxSec	86400
extern struct process *running_process;
extern struct queue *ready_queue;
/*
 * Global Variables
 */
//SINT32 Counter = 86390;
SINT32 Counter = 86390;
SINT32 Counter2 = 0;
SINT32 WC_Start_Counter = 0;
SINT32 TimeUpdated = 0;
SINT32 WC_Active = 1;
CHAR timeStr[11];
extern int CURR_TRAP;
void wc_process() 
{
	int hours = 0 ;
	int mins = 0 ;
	int secs  = 0;
	int i;
	while(TRUE)
	{
		int sender_ID = -1;
		rtx_dbug_outs("In WC1\r\n");
		TRACE("Calling receive_message()\r\n");
		void *envelope = request_memory_block();
		rtx_dbug_outs(itoa(envelope));
		if ( envelope == NULL ) 
			rtx_dbug_outs("In WC2\r\n");
		delayed_send(WC_PROCESS_ID, envelope , 950);
		rtx_dbug_outs("In WC3\r\n");
		envelope  = receive_message(&sender_ID);
		rtx_dbug_outs(itoa(envelope));
		rtx_dbug_outs("In WC4\r\n");
		rtx_dbug_outs(itoa(envelope));
		release_memory_block(envelope);
		rtx_dbug_outs("In WC5\r\n");
		Counter++;
		rtx_dbug_outs("In WC\r\n");
		
		// To FIX
		//char *msgData = (char *)(envelope + 64);
		//if ( msgData[0] == 'S' || msgData[0] == 's') {
		
		//}
		
		/*if ( TimeUpdated == 1 ) 
			TimeUpdated = 0;
		else release_processor();*/

		if ( Counter == MaxSec ) {
			Counter = 0;
		}
	
		secs = Counter % 60; 
		mins = Counter / 60;
		hours = mins / 60;
		mins = mins % 60;
	
		timeStr[0] = ( hours / 10 ) % 10 + 48;
		timeStr[1] = hours % 10 + 48;
		timeStr[2] = ':';
	
		timeStr[3] = ( mins / 10 ) % 10 + 48;
		timeStr[4] = mins % 10 + 48;
		timeStr[5] = ':';
	
		timeStr[6] = ( secs / 10 ) % 10 + 48;
		timeStr[7] = secs % 10 + 48;
		timeStr[8] = '\r';
		timeStr[9] = '\0';
		
		//rtx_dbug_outs(" Timer Val ");
		//rtx_dbug_outs((char*)timeStr);
		//rtx_dbug_outs(" \r\n");
		void *mem_block = request_memory_block();		
		for ( i = 0 ; i < 10 ; i++ ) {
			*(char *)(mem_block + 64 + i) = timeStr[i];
			}
		send_message(CRT_PROCESS_ID , mem_block);
		release_processor();

	}
}

/*
 * This function is called by the assembly STUB function
 */
VOID c_timer_handler( VOID )
{

	Counter2 += 10;
	TIMER0_TER = 2;/* Acknowledge interrupt */ 
	// Load timer - i process
	
	//save_context(running_process->ID);
	
	if ( delayed_send_queue->head != NULL ) {
	if ( delayed_send_queue->head->delay < Counter2 ) {
		rtx_dbug_outs (" In Timer Int " );
		int process_ID = delayed_send_queue->head->process_ID ;
		int sender_ID = delayed_send_queue->head->sender_id;
		void * MessageEnvelope = delayed_send_queue->head->MessageEnvelope;
		d_dequeue(delayed_send_queue);
		
		struct envelope *e = (struct envelope *)MessageEnvelope;
		e->sender_process_ID = sender_ID;
		e->dest_process_ID = process_ID;
	
		e->next = NULL;
		e->message = MessageEnvelope + MESSAGE_HEADER_OFFSET;

		struct process *dest_process;
		dest_process = get_proc(e->dest_process_ID);
	
		/* Add message to process mailbox */
		if (dest_process->mailbox_head == NULL)
			dest_process->mailbox_head = e;
		else
			dest_process->mailbox_tail->next = e;

		dest_process->mailbox_tail = e;

		/* Unblock destination process if it is blocked on receive */
		if(dest_process->state == STATE_BLOCKED 
			&& dest_process->block_type == BLOCK_RECEIVE)
		{
			unblock_process(e->dest_process_ID, NULL);
			scheduler_run();				
		}
		}
		}
	if (running_process->pending_sys_call != SYS_CALL_NONE)
	{
		CURR_TRAP = running_process->pending_sys_call;
		execute_sys_call();
		running_process->pending_sys_call = SYS_CALL_NONE;
	}	
	
	    //TIMER0_TRR = 0x6DDD;
	//TIMER0_TRR = 0x01C2;
	TIMER0_TRR = 0x1194;

    /*
     * Setup the timer prescaler and stuff
     */
    //TIMER0_TMR = 0x633D;
	TIMER0_TMR = 0x633B;
}

void timer_iprocess() {
	// Check_Delay_Send();
	while ( 1 ) {

	//TRACE (" .........................................In Timer I - Process \r\n " );
	if ( delayed_send_queue->head != NULL ) {
	if ( delayed_send_queue->head->delay < Counter2 ) {
		//rtx_dbug_outs(" ....................CHECKED...................................................CHECKED \r\n");
		int process_ID = delayed_send_queue->head->process_ID << 16 | delayed_send_queue->head->sender_id;
		void * MessageEnvelope = delayed_send_queue->head->MessageEnvelope;
		d_dequeue(delayed_send_queue);
		rtx_dbug_outs(itoa(		process_ID ));
		send_message( process_ID , MessageEnvelope );
	}
	}
	////rtx_dbug_outs (" .........................................Back In Timer I - Process \r\n " );
	/*if ( WC_Active ) 
	{
		WC_Start_Counter++;
		////rtx_dbug_outs(" ............. Due to Timer2 \r\n");
		if (  WC_Start_Counter % 1000 == 0) 
		{
			Counter++;
			TimeUpdated	= 1;
			//rtx_dbug_outs(" Due to Timer \r\n");
			/* Non-blocking call to request_memory_block() */
			/*if ( get_proc(WC_PROCESS_ID)->mailbox_head == NULL ) {
				//rtx_dbug_outs(" Due to Timer2 \r\n");
				void *mem_block = request_memory_block();
				if ( mem_block != NULL ) {
					*(char *)(mem_block + 64) = "";
					send_message(WC_PROCESS_ID, mem_block);
				}
			}*/
			
			//scheduler_run();
		//}
	//}
	
	//running_process->state = STATE_READY;
	//running_process = NULL;
	release_processor();
	}
	
}

SINT32 coldfire_vbr_init( VOID )
{
    /*
     * Move the VBR into real memory
     */
    asm( "move.l %a0, -(%a7)" );
    asm( "move.l #0x10000000, %a0 " );
    asm( "movec.l %a0, %vbr" );
    asm( "move.l (%a7)+, %a0" );
    
    return RTX_SUCCESS;
}

/*
 * Entry point, check with m68k-coff-nm
 */
void timer_init( void )
{
	TRACE("Timer Init");
    UINT32 mask;

    /* Disable all interupts */
    asm( "move.w #0x2700,%sr" );

    coldfire_vbr_init();
	//uart1_init();

    /*
     * Store the timer ISR at auto-vector #6
     */
    asm( "move.l #asm_timer_entry,%d0" );
    asm( "move.l %d0,0x10000078" );
	
    /*
     * Setup to use auto-vectored interupt level 6, priority 3
     */
    TIMER0_ICR = 0x9B;

    /*
     * Set the reference counts, ~10ms
     */
    //TIMER0_TRR = 0x6DDD;
	//TIMER0_TRR = 0x01C2;
	TIMER0_TRR = 0x1194;

    /*
     * Setup the timer prescaler and stuff
     */
    //TIMER0_TMR = 0x633D;
	TIMER0_TMR = 0x633B;

    /*
     * Set the interupt mask
     */
    mask = SIM_IMR;
    mask &= 0x0003fdff;
    SIM_IMR = mask;    

	TRACE("End Timer Init \r\n");
	
    /* Let the timer interrupt fire, lower running priority */
    //asm( "move.w #0x2700,%sr" );
	

}