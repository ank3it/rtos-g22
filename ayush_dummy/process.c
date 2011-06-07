/**
 * @group: S11-G22
 * @file: process.c
 * @date: 2011/05/22
 * @brief: Contains functions for manipulating processes
 * 	including the scheduler and context saving functions
 */

#include "process.h"
#include "init.h"


int counter;

/* ----- Testing functions ----- */
/* Reomve these later! */
int __main(void)
{
	return 0;
}

int main(void)
{
	rtx_dbug_outs((CHAR *) "process.c\r\n");
	init_pcb();
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
	
	rtx_dbug_outs(itoa(new_running_process_ID));
	rtx_dbug_outs((CHAR *) "\r\n");

	running_process = get_proc(new_running_process_ID);
	running_process->state = STATE_RUNNING;
	load_context(new_running_process_ID);
}

/**
 * @brief: Swaps the currently running process with the next one in the
 *	ready queue
 */
int k_release_processor()
{
	//switch between processes 
	//in order to get the previous process
	rtx_dbug_outs( (CHAR *) "\n\r Release Proc= " );
	save_context(running_process->ID);

	rtx_dbug_outs( (CHAR *) "\n\r Release Proc2= " );
	
	if(running_process->state == STATE_RUNNING){

		rtx_dbug_outs( (CHAR *) "\n\r Release Proc3= " );
		running_process->state = STATE_READY;
		enqueue(ready_queue, running_process->ID, running_process->priority);

		running_process = NULL;
	}

	rtx_dbug_outs( (CHAR *) "\n\r Release Proc4= " );
	scheduler_run();

	return RTX_SUCCESS;
}

void save_context(int process_ID)
{
	struct process * curr_process;
	curr_process = get_proc(process_ID);

//#ifdef CONTEXT_DEBUG
	rtx_dbug_outs( (CHAR *) "\n\r Current Process= " );
	rtx_dbug_outs( itoa(curr_process) );
//#endif
	
	//Push Data and Address Registers into the stack
	asm("move.l %%d0, %0" : "=m" (curr_process->d0));
	asm("move.l %%d1, %0" : "=m" (curr_process->d1));
	asm("move.l %%d2, %0" : "=m" (curr_process->d2));
	asm("move.l %%d3, %0" : "=m" (curr_process->d3));
	asm("move.l %%d4, %0" : "=m" (curr_process->d4));
	asm("move.l %%d5, %0" : "=m" (curr_process->d5));
	asm("move.l %%d6, %0" : "=m" (curr_process->d6));
	asm("move.l %%d7, %0" : "=m" (curr_process->d7));

	asm("move.l %%a0, %0" : "=m" (curr_process->a0));
	asm("move.l %%a1, %0" : "=m" (curr_process->a1));
	asm("move.l %%a2, %0" : "=m" (curr_process->a2));
	asm("move.l %%a3, %0" : "=m" (curr_process->a3));
	asm("move.l %%a4, %0" : "=m" (curr_process->a4));
	asm("move.l %%a5, %0" : "=m" (curr_process->a5));
	asm("move.l %%a6, %0" : "=m" (curr_process->a6));

	curr_process->curr_SP = curr_process->d5;
	curr_process->curr_SP -= 2;


	// This is temp ( Need to figure out the Variable or method to store this)
	//asm("move.l %%a7, %0" : "=m" (curr_process->curr_SP) ); 
	
}

void load_context(int process_ID)
{
	counter = 0;
	struct process *next_process = 
		next_process = get_proc(process_ID);
	int *sp = next_process->curr_SP;
	int put = next_process->a0;
	
//#ifdef CONTEXT_DEBUG
	rtx_dbug_outs( (CHAR *) "\n\r Next Process= " );
	rtx_dbug_outs( itoa(next_process) );
	rtx_dbug_outs( (CHAR *) "\n\r SP= " );
	rtx_dbug_outs( itoa(sp) );
//#endif

	if(sp != NULL)
	{
		if (next_process->state == STATE_READY)
		{
			rtx_dbug_outs( (CHAR *) "in Ready State\n\r" );
			asm("move.l %0, %%a0" : : "m" (put));
			put = next_process->a1;
			asm("move.l %0, %%a1" : : "m" (put));
			put = next_process->a2;
			asm("move.l %0, %%a2" : : "m" (put));
			put = next_process->a3;
			asm("move.l %0, %%a3" : : "m" (put));
			put = next_process->a4;
			asm("move.l %0, %%a4" : : "m" (put));
			put = next_process->a5;
			asm("move.l %0, %%a5" : : "m" (put));
			put = next_process->a6;
			asm("move.l %0, %%a6" : : "m" (put));
			put = next_process->d0;
			asm("move.l %0, %%d0" : : "m" (put));
			put = next_process->d1;
			asm("move.l %0, %%d1" : : "m" (put));
			put = next_process->d2;
			asm("move.l %0, %%d2" : : "m" (put));
			put = next_process->d3;
			asm("move.l %0, %%d3" : : "m" (put));
			put = next_process->d4;
			asm("move.l %0, %%d4" : : "m" (put));
			put = next_process->d5;
			asm("move.l %0, %%d5" : : "m" (put));
			put = next_process->d6;
			asm("move.l %0, %%d6" : : "m" (put));
			put = next_process->d7;
			asm("move.l %0, %%d7" : : "m" (put));
		}

			rtx_dbug_outs( (CHAR *) "Just Before ASm1!!\n\r" );
			asm("move.l %0, %%a7" : : "m" (sp));

			rtx_dbug_outs( (CHAR *) "Just Before ASm!!\n\r" );
			asm("rte");
	
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
	rtx_dbug_outs( (CHAR *) "\n\r In Set process Priority" );

	/* Check for invalid priorities */
	if (priority > PRIORITY_3 || priority < PRIORITY_0)
		return RTX_ERROR;

	/* Don't need to change anything */
	if (priority == k_get_process_priority(process_ID)) {
		rtx_dbug_outs( (CHAR *) "Exiting!!\n\r" );
		return RTX_SUCCESS;
	}

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
