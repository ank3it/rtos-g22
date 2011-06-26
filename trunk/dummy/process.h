/**
 * @group: S11-G22
 * @file: process.h
 * @date: 2011/05/22
 * @brief: Header file for process.c
 */

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "rtx_inc.h"
#include "dbug.h"
#include "util.h"
#include "malloc.h"
#include "defs.h"
#include "queue.h"
#include "envelope.h"

/*
 * Global variables
 */
struct process
{
	int ID;
	int priority;
	int state;
	int block_type;
	VOID (*entry)();
	int* curr_SP;
	int sz_stack;
	struct envelope *mailbox_head;
	struct envelope *mailbox_tail;
	int d0;
	int d1;
	int d2;
	int d3;
	int d4;
	int d5;
	int d6;
	int d7;
	int a0;
	int a1;
	int a2;
	int a3;
	int a4;
	int a5;
};

struct process all_processes[NUM_PROCS];
struct process *running_process;
struct queue *ready_queue;
struct queue *blocked_queue;

/* Initializae the scheduler */
void scheduler_init();

/* Select a process to run based on priority */
void scheduler_run();

/* Block the current running process */
void block_running_process(int);

/* Unblock either the given process, or the FIFO first process */
void unblock_process(int, int);

/* Yield the processor to another process */
int k_release_processor();

/* Save the current process's context */
void save_context(int);

/* Load the given process's context */
void load_context(int);

/* Retrieve the PCB of the given process */
struct process *get_proc(int);

/* Set process priority to given value */
int k_set_process_priority(int, int); 

/* Return priority for given process */
int k_get_process_priority(int);

/* Send message in message envelope to another process */
int k_send_message(int, void *);

/* Receive message from process mailbox */
void *k_receive_message(int *);

#endif /* _PROCESS_H_ */
