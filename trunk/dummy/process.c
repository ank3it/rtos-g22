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
	//running_process->state = STATE_RUNNING;
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
	save_context(running_process->ID);
	
	if(running_process->state == STATE_RUNNING){
		running_process->state = STATE_READY;
		//save_context(running_process->ID);
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

//#ifdef CONTEXT_DEBUG
	rtx_dbug_outs( (CHAR *) "\n\r Current Process= " );
	rtx_dbug_outs( itoa(curr_process) );
//#endif

	asm("move.l %%d5, %0" : "=m" (curr_process->curr_SP));	
}

void load_context(int process_ID)
{
	struct process *next_process = 
	next_process = get_proc(process_ID);
	int *sp = next_process->curr_SP;
	
	if(sp != NULL)
	{
		if (next_process->state == STATE_READY)
		{	next_process->state = STATE_RUNNING;
			rtx_dbug_outs( (CHAR *) "in Ready State\n\r" );
			asm("move.l %0, %%d5" : : "m" (sp));
			rtx_dbug_outs((CHAR*)"yes\n\r");
		}
		else {
			next_process->state = STATE_RUNNING;
			asm("move.l %0, %%a7" : : "m" (sp));
			asm("rte");
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
	rtx_dbug_outs( (CHAR *) "Scheduler Run!!\n\r" );

	return RTX_SUCCESS;
}

/**
 * @brief: Return the priority of a given process
 * @param: process_ID The process id of the process
 * @return: int Returns priority if successful, -1 otherwise
 */ 
int k_get_process_priority(int process_ID)
{
	rtx_dbug_outs(itoa(process_ID));
	return get_proc(process_ID)->priority;
}
