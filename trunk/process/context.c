#include "context.h"

process* curr_process;
process* next_process;
int* sp;

void save_context( int PID)
{
	curr_process = get_proc(PID);

#ifdef CONTEXT_DEBUG
	rtx_dbug_outs( (CHAR *) "\n\r Current Process= " );
	rtx_dbug_outs( itoa(curr_process) );
#endif
	
	//Push Data and Address Registers into the stack
	asm("move.l %d0, -(%a7)");
	asm("move.l %d1, -(%a7)");
	asm("move.l %d2, -(%a7)");
	asm("move.l %d3, -(%a7)");
	asm("move.l %d4, -(%a7)");
	asm("move.l %d5, -(%a7)");
	asm("move.l %d6, -(%a7)");
	asm("move.l %d7, -(%a7)");
	
	asm("move.l %a0, -(%a7)");
	asm("move.l %a1, -(%a7)");
	asm("move.l %a2, -(%a7)");
	asm("move.l %a3, -(%a7)");
	asm("move.l %a4, -(%a7)");
	asm("move.l %a5, -(%a7)");
	asm("move.l %a6, -(%a7)");
	
	asm("move.l %%a7, %0" : "=m" (curr_process->curr_SP) ); // This is temp (Need to figure out the Variable or method to store this)
	
}

void load_context(int PID) {
	next_process = get_proc(PID);
	sp = next_process->curr_SP;
	
#ifdef CONTEXT_DEBUG
	rtx_dbug_outs( (CHAR *) "\n\r Next Process= " );
	rtx_dbug_outs( itoa(next_process) );
#endif

	if(sp != NULL)
	{
		asm("move.l sp, %a7");  /* load a7 with stack_ptr*/
			
		asm("move.l (%a7)+, %a6");
		asm("move.l (%a7)+, %a5");
		asm("move.l (%a7)+, %a4");
		asm("move.l (%a7)+, %a3");
		asm("move.l (%a7)+, %a2");
		asm("move.l (%a7)+, %a1");
		asm("move.l (%a7)+, %a0");
		
		asm("move.l (%a7)+, %d7");
		asm("move.l (%a7)+, %d6");
		asm("move.l (%a7)+, %d5");
		asm("move.l (%a7)+, %d4");
		asm("move.l (%a7)+, %d3");
		asm("move.l (%a7)+, %d2");
		asm("move.l (%a7)+, %d1");
		asm("move.l (%a7)+, %d0");
		
		asm("move.l %%a7, %0" : "=m" (next_process->curr_SP) ); // This is temp ( Need to figure out the Variable or method to store this)
	}
}