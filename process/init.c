#include "init.h"

char* current_sp;

void load_null_process() {
	 all_processes[0].ID = 0;
     all_processes[i].priority = 4;
     all_processes[i].sz_stack = 512;
	 all_processes[i].entry = null_process();
	 all_processes[i].state = STATE_NEW;
	 all_processes[i].block_type = BLOCK_NONE;
	 // Increment the current_sp as this will the starting point of the stack of the next process
	 current_sp = malloc(all_processes[i].sz_stack) + all_processes[i].sz_stack;//current_sp+all_processes[i].sz_stack ; 
	 all_processes[i].curr_SP = current_sp;
	 
	 // Save the Exceprtion Stack Frame
	 *all_processes[i].curr_SP = all_processes[i].entry;
	 all_processes[i].curr_SP--;
	 *all_processes[i].curr_SP = 0x4000 << 16 | K_SR; // Value to be decided by sudhir for User Process
	 all_processes[i].curr_SP-;
	 
}

void load_test_processes() {
	 /* get the third party test proc initialization info */
    __REGISTER_TEST_PROCS_ENTRY__();

	// Save Test Processes
    for (i =1; i< NUM_TEST_PROCS+1; i++ ) {
        all_processes[i].ID = g_test_proc[i].pid;
        all_processes[i].priority = g_test_proc[i].priority;
        all_processes[i].sz_stack = g_test_proc[i].sz_stack;
		all_processes[i].entry = g_test_proc[0].entry;
		all_processes[i].state = STATE_NEW;
		all_processes[i].block_type = BLOCK_NONE;
		// Increment the current_sp as this will the starting point of the stack
		current_sp = current_sp+all_processes[i].sz_stack ; 
		all_processes[i].curr_SP = current_sp;
	 
		// Save the Exceprtion Stack Frame
		*all_processes[i].curr_SP = all_processes[i].entry;
		all_processes[i].curr_SP--;
		*all_processes[i].curr_SP = 0x4000 << 16 | U_SR; // Value to be decided by sudhir for User Process
		all_processes[i].curr_SP-;
    }
}

void init_pcb()
{
	int i;

    rtx_dbug_outs((CHAR *)"In init_pcb() \r\n");

	init_memory();
	//timer_init();

	set_current_sp();// This will be set in memory.c at the end of memory pool
	
	// Null Process
	load_null_process();

	load_test_processes();
}

process* get_proc(int ID)
{
	return &(all_processes[ID]);
}