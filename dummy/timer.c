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
void wc_process() 
{
	int hours = 0 ;
	int mins = 0 ;
	int secs  = 0;
	//while(TRUE)
	//{
		int sender_ID = -1;
		TRACE("Calling receive_message()\r\n");
		//void * envelope = receive_message(&sender_ID);
		
		// To FIX
		//char *msgData = (char *)(envelope + 64);
		//if ( msgData[0] == 'S' || msgData[0] == 's') {
		
		//}
		
		if ( TimeUpdated == 1 ) TimeUpdated = 0;
		else release_processor();

		CHAR timeStr[] = "00:00:00\r";
	
		if ( Counter == MaxSec ) {
			Counter = 0;
		}
	
		secs = Counter % 60;
		mins = Counter / 60;
		hours = mins / 60;
		mins = mins % 60;
	
		timeStr[0] = ( hours / 10 ) % 10 + 48;
		timeStr[1] = hours % 10 + 48;
	
		timeStr[3] = ( mins / 10 ) % 10 + 48;
		timeStr[4] = mins % 10 + 48;
	
		timeStr[6] = ( secs / 10 ) % 10 + 48;
		timeStr[7] = secs % 10 + 48;
		
		//rtx_dbug_outs(" Timer Val ");
		//rtx_dbug_outs((char*)timeStr);
		//rtx_dbug_outs(" \r\n");
		void *mem_block = request_memory_block();
		*(char *)(mem_block + 64) = timeStr;
		//CharEntered = 1;
		//CharIn = SERIAL1_RD;
		k_send_message(CRT_PROCESS_ID , mem_block);
		release_memory_block(mem_block);

	//}
}

/*
 * This function is called by the assembly STUB function
 */
VOID c_timer_handler( VOID )
{
	asm( "move.w #0x2700,%sr" );				// Re-Enable all interrupts
	Counter2++;	
	// Load timer - i process
	//TRACE (" In Timer Int " );
	save_context(running_process->ID);
	
	if(running_process->state == STATE_RUNNING){
		running_process->state = STATE_READY;

		enqueue(ready_queue, 
				running_process->ID, 
				running_process->priority);

		running_process = NULL;
	}
	
	running_process = get_proc(TIMER_IPROCESS_ID);
	
	load_context(TIMER_IPROCESS_ID);
	
	TIMER0_TRR = 0x6DDD;
	//TIMER0_TRR = 0x01C2;

    /*
     * Setup the timer prescaler and stuff
     */
    TIMER0_TMR = 0x633D;
	//TIMER0_TMR = 0x633B;

	TIMER0_TER = 2;/* Acknowledge interrupt */ 
	 
	asm( "move.w #0x2000,%sr" );				// Re-Enable all interrupts
}

void timer_iprocess() {
	// Check_Delay_Send();
	while ( 1 ) {

	TRACE (" .........................................In Timer I - Process \r\n " );
	if ( delayed_send_queue->head != NULL ) {
	if ( delayed_send_queue->head->delay < Counter2 ) {
		//TRACE(" ....................CHECKED...................................................CHECKED \r\n");
		int process_ID = delayed_send_queue->head->process_ID << 16 | delayed_send_queue->head->sender_id;
		void * MessageEnvelope = delayed_send_queue->head->MessageEnvelope;
		d_dequeue(delayed_send_queue);		
		send_message( process_ID , MessageEnvelope );
	}
	}
	rtx_dbug_outs (" .........................................Back In Timer I - Process \r\n " );
	if ( WC_Active ) 
	{
		WC_Start_Counter++;
		//rtx_dbug_outs(" ............. Due to Timer2 \r\n");
		if ( WC_Start_Counter % 1000 == 0) 
		{
			Counter++;
			TimeUpdated	= 1;
			//rtx_dbug_outs(" ............. Due to Timer \r\n");
			//wc_process();
			
			//scheduler_run();
		}
	}
	
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
    TIMER0_TRR = 0x6DDD;
	//TIMER0_TRR = 0x01C2;

    /*
     * Setup the timer prescaler and stuff
     */
    TIMER0_TMR = 0x633D;
	//TIMER0_TMR = 0x633B;

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