#include "init.h"
#include "rtx_test.h"
#include "rtx.h"
#include "malloc.h"
#include "trap.h"

char* current_sp;
extern int __end;
int* blah;

void null_process()
{
	while(1)
	{
		TRACE("This is inside the Null Process\r\n");
		//release_processor();
	}
}

void load_null_process() {
	
	TRACE("load_null_process()\r\n");

	all_processes[0].ID = 0;
	all_processes[0].priority = 4;
	all_processes[0].sz_stack = 512;
	all_processes[0].entry = null_process;
	all_processes[0].state = STATE_NEW;
	all_processes[0].block_type = BLOCK_NONE;
	// Increment the current_sp as this will the starting point of the stack of the next process
	current_sp = malloc(all_processes[0].sz_stack) + all_processes[0].sz_stack;//current_sp+all_processes[i].sz_stack ; 
	all_processes[0].curr_SP = current_sp;

	// Save the Exceprtion Stack Frame
	*all_processes[0].curr_SP = all_processes[0].entry;
	all_processes[0].curr_SP--;
	*all_processes[0].curr_SP = 0x4000 << 16 | K_SR; // Value to be decided by sudhir for User Process

	enqueue(ready_queue , all_processes[0].ID , all_processes[0].priority );
}

void load_test_processes() {

	TRACE("load_test_processes()\r\n");	
	
	int i;

	 /* get the third party test proc initialization info */
    __REGISTER_TEST_PROCS_ENTRY__();

	// Save Test Processes
    for (i =1; i< NUM_TEST_PROCS+1; i++ ) {
        all_processes[i].ID = g_test_proc[i-1].pid;
        all_processes[i].priority = g_test_proc[i-1].priority;
        all_processes[i].sz_stack = g_test_proc[i-1].sz_stack;
		all_processes[i].entry = g_test_proc[i-1].entry;
		all_processes[i].state = STATE_NEW;
		all_processes[i].block_type = BLOCK_NONE;
		// Increment the current_sp as this will the starting point of the stack
		current_sp = malloc(all_processes[i].sz_stack) + all_processes[i].sz_stack; 
		all_processes[i].curr_SP = current_sp;
		all_processes[i].mailbox_head = NULL;
		all_processes[i].mailbox_tail = NULL;
	 
		// Save the Exceprtion Stack Frame
		all_processes[i].curr_SP--;
		*all_processes[i].curr_SP = all_processes[i].entry;
		all_processes[i].curr_SP--;
		*all_processes[i].curr_SP = 0x4000 << 16 | U_SR; // Value to be decided by sudhir for User Process
		//all_processes[i].curr_SP--;
		enqueue(ready_queue , all_processes[i].ID , all_processes[i].priority );
    }

	/*
	for (i =0; i< 10; i++ ) {
		TRACE("\r\nProcess= " );
		struct process *next_process = &all_processes[i];
		int *sp = all_processes[i].curr_SP;
		int *ep = all_processes[i].entry;
		TRACE(itoa((next_process)));
		TRACE("\r\nSP= " );
		TRACE(itoa((sp)));
		TRACE("\r\nEP= " );
		TRACE(itoa((ep)));
	}
	*/
}

void init_pcb()
{
	TRACE("init_pcb()\r\n");

	int i;

	malloc_init(&(__end));
	scheduler_init();
    init_trap();

	init_memory();
	//timer_init();
	
	// Null Process
	load_null_process();

	load_test_processes();

//	init_funcs();

	scheduler_run();
}

void  __attribute__ ((section ("__REGISTER_RTX__"))) init_funcs() {
	TRACE("init_funcs()\r\n");

	g_test_fixture.send_message = send_message;
    g_test_fixture.receive_message = receive_message;
    g_test_fixture.request_memory_block = request_memory_block;
    g_test_fixture.release_memory_block = release_memory_block;
    g_test_fixture.release_processor = release_processor;
    g_test_fixture.delayed_send = delayed_send;
    g_test_fixture.set_process_priority = set_process_priority;
    g_test_fixture.get_process_priority = get_process_priority;
}

int __main(void)
{
	return 0;
}

int main(void)
{
	TRACE("main() of init.c\r\n");
	init_pcb();
	return 0;
}
