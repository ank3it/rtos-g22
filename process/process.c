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
extern BYTE __end;

void null_process(void)
{
	trace((CHAR *) "null_process");
}

int __main(void)
{
	return 0;
}

int main(void)
{
	trace((CHAR *) "process.c");

	malloc_init(&__end);
	scheduler_init();

	trace((CHAR *) "scheduler initialized");
	
	all_processes[0].ID = NULL_PROCESS_ID;
	all_processes[0].priority = 4;
	all_processes[0].sz_stack = 512;
	all_processes[0].entry = null_process;
	all_processes[0].state = STATE_NEW;
	all_processes[0].block_type = BLOCK_NONE;
	all_processes[0].curr_SP = 
		malloc(all_processes[0].sz_stack) + all_processes[0].sz_stack;

	*all_processes[0].curr_SP = all_processes[0].entry;
	all_processes[0].curr_SP--;
	*all_processes[0].curr_SP = 0x4000 << 16 | K_SR;
	all_processes[0].curr_SP--;

	trace((CHAR *) "process created");
	enqueue(ready_queue, all_processes[0].ID, all_processes[0].priority);
	trace((CHAR *) "process enqueued");

	trace(itoa((int)ready_queue->head->priority));
	scheduler_run();
	trace((CHAR *) "scheduler ran");

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
	if (ready_queue->head == NULL
		|| running_process->priority <= ready_queue->head->priority)
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
	int new_running_process_ID = dequeue(ready_queue);

	running_process = get_proc(new_running_process_ID);
	load_context(new_running_process_ID);
	running_process->state = STATE_RUNNING;
}

/**
 * @brief: Swaps the currently running process with the next one in the
 *	ready queue
 */
int k_release_process()
{
	//switch between processes 
	//in order to get the previous process
	save_context(running_process->ID);
	
	if(running_process->state == STATE_RUNNING){
		running_process->state = STATE_READY;
		enqueue(ready_queue, running_process->ID, running_process->priority);

		running_process = NULL;
	}

	scheduler_run();

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

	// This is temp ( Need to figure out the Variable or method to store this)
	asm("move.l %%a7, %0" : "=m" (curr_process->curr_SP) ); 
	
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
		/* load a7 with stack_ptr*/
		asm("move.l %0, %%a7" : : "m" (sp));  			

		if (next_process->state == STATE_READY)
		{
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

			// This is temp ( Need to figure out the Variable or method to store this)
			asm("move.l %%a7, %0" : "=m" (next_process->curr_SP) ); 
		}
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
