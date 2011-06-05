/**
 * @group: S11-G22
 * @file: process.c
 * @date: 2011/05/22
 * @brief: Contains functions for manipulating processes
 * 	including the scheduler and context saving functions
 */

#include "process.h"

/* ----- Testing functions ----- */
/* Reomve these later! */
int __main(void)
{
	return 0;
}

int main(void)
{
	rtx_dbug_outs((CHAR *) "process.c\r\n");
	return 0;
}
/* ----- End testing functions ----- */

/**
 * @brief: Initialize scheduler by creating ready and blocked 
 *	queues and setting the running process to NULL
 */
void scheduler_init()
{
	ready_queue = malloc(sizeof(struct queue));
	queue_init(ready_queue);

	blocked_queue = malloc(sizeof(struct queue));
	queue_init(blocked_queue);

	running_process = NULL;
}

/**
 * @brief: Select a process to run based on priority
 */
void scheduler_run()
{
	/* Scheduler:
	 * - Check if running_process is NULL
	 * - if true then dequeue ready queue, load process context
	 *	and set as running process
	 * - if false then compare running process priority with 
	 *	ready head priority
	 * - if running priority is less, then do nothing
	 * - if running priority is greater, then: 
	 *		- save running context
	 *		- enqueue running process onto ready queue 
	 *		- dequeue head of ready queue
	 *		- set as runnning process
	 *		- load context of new running process
	 */

	if (running_process->priority <= ready_queue->head->priority)
		return;

	if (running_process != NULL)
	{
		/* Swap out runnning process */
		save_context(running_process->ID);
		enqueue(ready_queue, 
				running_process->ID, 
				running_process->priority);
		running_process->state = STATE_READY;
	}

	/* Swap in new running process */
	int new_running_process_ID = ready_queue->head->value;

	/* Don't dequeue the null process from the ready queue */
	if (new_running_process_ID != NULL_PROCESS_ID)
		dequeue(ready_queue);

	running_process = get_proc(new_running_process_ID);
	load_context(new_running_process_ID);
	running_process->state = STATE_RUNNING;
}

int k_release_process()
{
	/*PUT A BREAK-POINT here and check what process we get after calling a TRAP*/
	
	//if new state
	/*if(curr_process.state == STATE_NEW){
		load_context(curr_process.ID);
	}*/	

			
	//switch between processes 
	//in order to get the previous process
	save_context(running_process->ID);
	
	//if blocked state
	/*if(curr_process->state == STATE_BLOCKED){
		
		//add to blocked queue
		add_to_blocked(curr_process->ID);
		
	}*/
	
	if(running_process->state == STATE_READY){
		enqueue(ready_queue, running_process->ID, running_process->priority);
	}

	//scheduler_run();

	return RTX_SUCCESS;
}

void save_context(int process_ID)
{
	struct process * curr_process;
	curr_process = get_proc(process_ID);

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

void load_context(int process_ID)
{
	struct process *next_process = 
		next_process = get_proc(process_ID);
	int *sp = next_process->curr_SP;
	
#ifdef CONTEXT_DEBUG
	rtx_dbug_outs( (CHAR *) "\n\r Next Process= " );
	rtx_dbug_outs( itoa(next_process) );
#endif

	if(sp != NULL)
	{
		//asm("move.l sp, %a7");  /* load a7 with stack_ptr*/
			
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

struct process * get_proc(int process_ID)
{
	return &(all_processes[process_ID]);
}

/**
 * @brief: Set the process priority 
 * @param: process_ID Process to be modified
 * @param: priority The priority the process will be set to
 * @return: int Returns 0 on success, -1 otherwise
 */
int k_set_process_priority(int process_ID, int priority)
{
	/* Check for invalid priorities */
	if (priority > PRIORITY_3 || priority < PRIORITY_0)
		return RTX_ERROR;

	/* Don't need to change anything */
	if (priority == k_get_process_priority(process_ID))
		return RTX_SUCCESS;

	int state = get_proc(process_ID)->state;

	if (state == STATE_RUNNING)
	{
		get_proc(process_ID)->priority = priority;
	}
	else if (state == STATE_BLOCKED)
	{
		remove(blocked_queue, process_ID);
		get_proc(process_ID)->priority = priority;
		enqueue(blocked_queue, process_ID, priority);
	}
	else
	{
		remove(ready_queue, process_ID);
		get_proc(process_ID)->priority = priority;
		enqueue(ready_queue, process_ID, priority);
	}

	/* Run scheduler in case a process needs to be pre-empted */
	scheduler_run();

	return RTX_SUCCESS;
}

/**
 * @brief: Return the priority of a given process
 * @param: process_ID The process id of the process
 * @return: int Returns priority if successful, -1 otherwise
 */ 
int k_get_process_priority(int process_ID)
{
	return get_proc(process_ID)->priority;
}
