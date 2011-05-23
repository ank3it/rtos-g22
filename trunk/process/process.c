/**
 * @group: S11-G22
 * @file: process.c
 * @date: 2011/05/22
 * @brief: Contains functions for manipulating processes
 */

#include "process.h"

/*
 * Global variables
 */
typedef struct
{
	int ID;
	int priority;
	int state;
	int block_type;
} process;

process *ready_head;
process *ready_tail;
process *blocked_head;
process *blocked_tail;

/* Testing functions (Move to external file) */
int __main( void )
{
	return 0;
}

int main( void )
{
	rtx_dbug_outs( ( CHAR * ) "Inside process main()" );

	return 0;
}
/* End of testing functions */

int set_process_priority( int process_ID, int priority )
{
	/* Set the priority of the given process */
	return 0;
}

int get_process_priority( int process_ID )
{
	/* Return the given process's id */
	return -1;
}
