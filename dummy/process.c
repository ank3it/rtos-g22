/**
 * @group: S11-G22
 * @file: process.c
 * @date: 2011/05/22
 * @brief: Contains functions for manipulating processes
 * 	including the scheduler and context saving functions
 */

#include "process.h"
#include "init.h"

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
	
	TRACE(itoa(new_running_process_ID));
	TRACE("\r\n");

	running_process = get_proc(new_running_process_ID);
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

		enqueue(ready_queue, 
				running_process->ID, 
				running_process->priority);

		running_process = NULL;
	}

	scheduler_run();

	return RTX_SUCCESS;
}

void save_context(int process_ID)
{
	struct process * curr_process;
	curr_process = get_proc(process_ID);

	TRACE("\n\r Current Process= " );
	TRACE( itoa(curr_process) );

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
			TRACE("in Ready State\n\r" );
			asm("move.l %0, %%d5" : : "m" (sp));
			TRACE("yes\n\r");
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
	TRACE("\n\r In Set process Priority" );

	/* Check for invalid priorities */
	if (priority > PRIORITY_3 || priority < PRIORITY_0)
		return RTX_ERROR;

	/* Don't need to change anything */
	if (priority == k_get_process_priority(process_ID)) {
		TRACE("Exiting!!\n\r" );
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
	TRACE("Scheduler Run!!\n\r" );

	return RTX_SUCCESS;
}

/**
 * @brief: Return the priority of a given process
 * @param: process_ID The process id of the process
 * @return: int Returns priority if successful, -1 otherwise
 */ 
int k_get_process_priority(int process_ID)
{
	TRACE(itoa(process_ID));
	return get_proc(process_ID)->priority;
}

/**
 * @brief: Send a message in a message envelope to another process
 * @param: process_ID The ID of the process the message is being sent to
 * @param: message_envelope Memory data block containing the message envelope
 * @return: 0 on success, -1 otherwise
 */
int k_send_message(int process_ID, void *message_envelope)
{	
	struct envelope *e;
	e->message = message_envelope;
	
	/*
	e->sender_process_ID = message_envelope->sender_process_ID;
	e->dest_process_ID = message_envelope->dest_process_ID;
	*/
	
	struct process *send_process;
	send_process = get_proc(process_ID);
	
	if(send_process->state == STATE_BLOCKED && send_process->block_type == BLOCK_RECEIVE)
	{
		send_process->mailbox_head = e;
		
		remove(blocked_queue, process_ID);		 //i'm not sure about this, should i be removing it from the blocked queue?
		enqueue(ready_queue, 
					send_process->ID, 
					send_process->priority);
		scheduler_run();
	}else{
		
		if (send_process->mailbox_head == send_process->mailbox_tail){
				send_process->mailbox_head = e;
				send_process->mailbox_tail = NULL;
		}

		e->next = send_process->mailbox_head;
		send_process->mailbox_head = e;
	}
}

/**
 * @brief: Receive message from process mailbox. Blocks the calling process
 * 	if the maailbox is empty
 * @param: sender_ID Filled by the function with the message sender's ID
 * @return: Pointer to a message envelope struct
 */
void *k_receive_message(int *sender_ID)
{  
        // check mailbox of current process... block it if
        // it isn't currently blocked so that it's expecting a message
        if(running_process->mailbox_head == NULL)
        {
			/* Mailbox is empty! Block the process */
			save_context(running_process->ID);
			running_process->state = STATE_BLOCKED;
			running_process->block_type = BLOCK_RECEIVE;

			enqueue(blocked_queue, 
					running_process->ID, 
					running_process->priority);

			running_process = NULL;
			scheduler_run();

			return NULL;
        }
        else
        {
        	/* else grab the first message and remove it from the mailbox */
			struct envelope *e = running_process->mailbox_head;
			/* If popping mailbox with one element */
			if (running_process->mailbox_head == running_process->mailbox_tail)
				running_process->mailbox_tail = NULL;

			running_process->mailbox_head = e->next;

			/* Set sender_ID return parameter and return pointer to envelope */
			sender_ID = e->sender_process_ID;
			return e;
    	}
}
