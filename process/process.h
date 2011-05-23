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

#define NULL_PROCESS_ID			0;

/* Process Priorities */
#define PRIORITY_0				0;
#define PRIORITY_1				1;
#define PRIORITY_2				2;
#define PRIORITY_3				3;
#define NULL_PROCESS_PRIORITY	4;

/* Process states */
#define BLOCKED_STATE			0;
#define READY_STATE				1;

/* Set process priority to given value */
int set_process_priority( int process_ID, int priority ); 

/* Return priority for given process */
int get_process_priority( int process_ID );

#endif /* _PROCESS_H_ */
