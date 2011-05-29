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

typedef struct
{
	int val;
	process *process_ptr;
	void *next_ptr;
} queue_node;

extern BYTE __end;
queue_node *head;
queue_node *tail;

/* Testing functions (Move to external file) */
int __main(void)
{
	return 0;
}

int main(void)
{
	rtx_dbug_outs((CHAR *) "Inside process main()\r\n");

	int size = 10;

	/* Create first node */
    queue_node node;
    node.val = 0;
    node.process_ptr = 0;
    node.next_ptr = 0;

	/* Set head to point to first node */
    head = &node;

    /* Create the rest of the nodes */
    int i = 1;
    while (i < size)
    {
        rtx_dbug_outs((CHAR *) "Creating node: ");
        rtx_dbug_outs(itoa(i));
        rtx_dbug_outs((CHAR *) "\r\n");

        /* Create process
        process proc;
        proc.ID = i;
        proc.priority = PRIORITY_3;
        proc.state = STATE_NEW;
        proc.block_type = BLOCK_NONE;
        */

        /* Create node */
        queue_node next_node;
        next_node.val = i;
        next_node.process_ptr = 0;
        next_node.next_ptr = 0;

        node.next_ptr = &next_node;

        node = next_node;

        i++;
    }

    /* Set tail to point to last node */
    tail = &node;


	return 0;
}
/* End of testing functions */

/**
 * @brief: Create the ready and blocked queue
 * @param: size The size of the queues
 * @return: int Returns 0 on success, -1 otherwise
 */
int create_queues(int size)
{
	if (size < 1)
	{
		return RTX_ERROR;
	}

	return RTX_ERROR;
}

/**
 * @brief: Add process to ready queue 
 * @param: process_ID The process to be added
 * @return: int Returns 0 on success, -1 otherwise
 */
int add_to_ready(int process_ID)
{
	return RTX_ERROR;
}

/**
 * @brief: Add process to blocked queue
 * @param: process_ID The process to be added
 * @return: int Returns 0 on success, -1 otherwise
 */
int add_to_blocked(int process_ID)
{
	return RTX_ERROR;
}

/**
 * @brief: Set the process priority 
 * @param: process_ID Process to be modified
 * @param: priority The priority the process will be set to
 * @return: int Returns 0 on success, -1 otherwise
 */
int set_process_priority(int process_ID, int priority)
{
	if (process_ID == NULL_PROCESS_ID || priority > PRIORITY_3 ||
		priority < PRIORITY_0)
	{
		return RTX_ERROR;
	}

	return RTX_ERROR;
}

/**
 * @brief: Return the priority of a given process
 * @param: process_ID The process id of the process
 * @return: int Returns priority if successful, -1 otherwise
 */ 
int get_process_priority(int process_ID)
{
	/* Return the given process's id */
	return RTX_ERROR;
}
