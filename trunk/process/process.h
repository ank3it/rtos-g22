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

#define NULL_PROCESS_ID			0

/* Process Priorities */
#define PRIORITY_0				0
#define PRIORITY_1				1
#define PRIORITY_2				2
#define PRIORITY_3				3
#define NULL_PROCESS_PRIORITY	4

/* Process states */
#define STATE_NEW				0
#define STATE_READY				1
#define STATE_BLOCKED			2

/* Blocked types */
#define BLOCK_NONE				0
#define BLOCK_MEMORY			1
#define BLOCK_RECEIVE			2

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
