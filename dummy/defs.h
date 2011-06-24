/**
 * @group: S11-G22
 * @file: defs.h
 * @date: 2011/06/04
 * @brief: Defines
 */

#ifndef _DEFS_H_
#define _DEFS_H_

#define NUM_PROCS 10
#define K_SR 0x2700
#define U_SR 0x0

// Constant Defines
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
#define STATE_RUNNING			2
#define STATE_BLOCKED			3

/* Blocked types */
#define BLOCK_NONE				0
#define BLOCK_MEMORY			1
#define BLOCK_RECEIVE			2

#endif /* _DEFS_H_ */
