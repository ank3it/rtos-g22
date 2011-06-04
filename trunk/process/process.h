/**
 * @group: S11-G22
 * @file: process.h
 * @date: 2011/05/22
 * @brief: Header file for process.c
 */

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "../shared/rtx_inc.h"
#include "../dbug/dbug.h"
#include "../util/util.h"
#include "context.h"
#include "defs.h"

/*
 * Global variables
 */
typedef struct
{
	int ID;
	int priority;
	int state;
	int block_type;
	// char* process_data; // Removed as when we call memory_blocks, pointers to them will get stored in local variables which will automatically get pushed on top of the stack
	VOID   (*entry)();
	int* curr_SP;
	int sz_stack;
} process;

process all_processes[NUM_PROCS];

/* Create ready and blocked queues of given size */
int create_queues(int size);

/* Add process to ready queue */
int add_to_ready(int process_ID);

/* Add process to blocked queue */
int add_to_blocked(int processs_ID);

/* Set process priority to given value */
int set_process_priority(int process_ID, int priority); 

/* Return priority for given process */
int get_process_priority(int process_ID);

#endif /* _PROCESS_H_ */
