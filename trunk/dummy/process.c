/**
 * @group: S11-G22
 * @file: process.c
 * @date: 2011/05/22
 * @brief: Contains functions for manipulating processes
 * 	including the scheduler and context saving functions
 */

#include "process.h"
#include "init.h"

#define MESSAGE_HEADER_OFFSET 64

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

	return RTX_SUCCESS;
}

void save_context(int process_ID)
{
	TRACE("----- Saving context -----\r\n");
	struct process * curr_process;
	curr_process = get_proc(process_ID);

	TRACE("Current Process= " );
	TRACE( itoa(curr_process) );
	TRACE("\r\n");

	asm("move.l %%d5, %0" : "=m" (curr_process->curr_SP));

	asm("move.l %%d0, %0" : "=m" (curr_process->d0));
	asm("move.l %%d1, %0" : "=m" (curr_process->d1));
	asm("move.l %%d2, %0" : "=m" (curr_process->d2));
	asm("move.l %%d3, %0" : "=m" (curr_process->d3));
	asm("move.l %%d4, %0" : "=m" (curr_process->d4));
	asm("move.l %%d6, %0" : "=m" (curr_process->d6));
	asm("move.l %%d7, %0" : "=m" (curr_process->d7));
	asm("move.l %%a0, %0" : "=m" (curr_process->a0));
	asm("move.l %%a1, %0" : "=m" (curr_process->a1));
	asm("move.l %%a2, %0" : "=m" (curr_process->a2));
	asm("move.l %%a3, %0" : "=m" (curr_process->a3));
	asm("move.l %%a4, %0" : "=m" (curr_process->a4));
	asm("move.l %%a5, %0" : "=m" (curr_process->a5));
}

void load_context(int process_ID)
{
	TRACE("----- Loading context -----\r\n");
	struct process *next_process = 
	next_process = get_proc(process_ID);
	int *sp = next_process->curr_SP;
	
	if(sp != NULL)
	{
		if (next_process->state == STATE_READY)
		{	next_process->state = STATE_RUNNING;
			TRACE("Loading context of ready process\n\r" );
			asm("move.l %0, %%d5" : : "m" (sp));

			asm("move.l %0, %%d0" : : "m" (next_process->d0));
			asm("move.l %0, %%d1" : : "m" (next_process->d1));
			asm("move.l %0, %%d2" : : "m" (next_process->d2));
			asm("move.l %0, %%d3" : : "m" (next_process->d3));
			asm("move.l %0, %%d4" : : "m" (next_process->d4));
			asm("move.l %0, %%d6" : : "m" (next_process->d6));
			asm("move.l %0, %%d7" : : "m" (next_process->d7));
			asm("move.l %0, %%a0" : : "m" (next_process->a0));
			asm("move.l %0, %%a1" : : "m" (next_process->a1));
			asm("move.l %0, %%a2" : : "m" (next_process->a2));
			asm("move.l %0, %%a3" : : "m" (next_process->a3));
			asm("move.l %0, %%a4" : : "m" (next_process->a4));
			asm("move.l %0, %%a5" : : "m" (next_process->a5));
		}
		else {
			TRACE("Loading context of new process\r\n");
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
	/* Check if given parameters are invalid */
	if (process_ID >= NUM_PROCS || process_ID < 0 || message_envelope == NULL)
		return RTX_ERROR;

	/* Populate header fields */
	struct envelope *e = (struct envelope *)message_envelope;
	e->sender_process_ID = running_process->ID;
	e->dest_process_ID = process_ID;
	e->next = NULL;
	e->message = message_envelope + MESSAGE_HEADER_OFFSET;
	
	struct process *dest_process;
	dest_process = get_proc(process_ID);
	
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
		dest_process->state = STATE_READY;
		dest_process->block_type = BLOCK_NONE;
		remove(blocked_queue, process_ID);
		enqueue(ready_queue, 
				dest_process->ID, 
				dest_process->priority);
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
	TRACE("k_receive_message()\r\n");
	// check mailbox of current process... block it if
	// it isn't currently blocked so that it's expecting a message
	if(running_process->mailbox_head == NULL)
	{
		TRACE("Mailbox is empty!\r\n");

		/* Mailbox is empty! Block the process */
		save_context(running_process->ID);
		running_process->state = STATE_BLOCKED;
		running_process->block_type = BLOCK_RECEIVE;

		enqueue(blocked_queue, 
				running_process->ID, 
				running_process->priority);

		running_process = NULL;

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
		if (sender_ID != NULL)
			*sender_ID = e->sender_process_ID;
		return e;
	}
}
