/**
 * @group: S11-G22
 * @file: defs.h
 * @date: 2011/06/04
 * @brief: Defines
 */

#ifndef _DEFS_H_
#define _DEFS_H_

#define MAX_USER_MEM_BLKS 32
#define NUM_MEM_BLKS 100
#define MAX_BLK_SIZE 128

#define NUM_PROCS 16
#define K_SR 0x2700
#define U_SR 0x0

#define MAX_IDENTIFIER_LENGTH	16
#define KCD_BUFFER_SIZE			64

/* System process IDs */
#define NULL_PROCESS_ID			0
#define PROCESS_A_ID			7
#define PROCESS_B_ID			8
#define PROCESS_C_ID			9
#define KCD_PROCESS_ID			10
#define CRT_PROCESS_ID			11
#define UART_IPROCESS_ID		12
#define TIMER_IPROCESS_ID		13
#define WC_PROCESS_ID			14
#define SPPC_PROCESS_ID			15


/* Process Priorities */
#define PRIORITY_0				0
#define PRIORITY_1				1
#define PRIORITY_2				2
#define PRIORITY_3				3

#define NULL_PROCESS_PRIORITY	4
#define UART_IPROCESS_PRIORITY	0
#define TIMER_IPROCESS_PRIORITY	0
#define WC_PROCESS_PRIORITY		0
#define SPPC_PROCESS_PRIORITY	0
#define KCD_PROCESS_PRIORITY	0
#define CRT_PROCESS_PRIORITY	0

/*User Process  priority variables */
#define A_PROCESS_PRIORITY		2
#define B_PROCESS_PRIORITY		2
#define C_PROCESS_PRIORITY		1

/* Process states */
#define STATE_NEW				0
#define STATE_READY				1
#define STATE_RUNNING			2
#define STATE_BLOCKED			3

/* Blocked types */
#define BLOCK_NONE				0
#define BLOCK_MEMORY			1
#define BLOCK_RECEIVE			2

/* Case values for function selection in trap handler */
#define SYS_CALL_NONE					-1
#define SYS_CALL_SEND_MESSAGE			0
#define SYS_CALL_RECEIVE_MESSAGE		1
#define SYS_CALL_REQUEST_MEMORY_BLOCK	2
#define SYS_CALL_RELEASE_MEMORY_BLOCK	3
#define	SYS_CALL_RELEASE_PROCESSOR		4
#define SYS_CALL_DELAYED_SEND			5
#define SYS_CALL_SET_PROCESS_PRIORITY	6
#define SYS_CALL_GET_PROCESS_PRIORITY	7

/* Constants for User Test Processes */
#define COUNT_REPORT			0
#define WAKEUP10				1

/* Command identifier used by the KCD */
#define COMMAND_PROMPT			'%'

#define MESSAGE_HEADER_OFFSET 64

#endif /* _DEFS_H_ */
