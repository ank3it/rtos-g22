/*--------------------------------------------------------------------------
 *                      RTX Test Suite 
 *--------------------------------------------------------------------------
 */
/**
 * @file:   rtx_test_dummy.c   
 * @author: Thomas Reidemeister
 * @author: Irene Huang
 * @date:   2010.02.11
 * @brief:  rtx test suite 
 */

#include "rtx_test.h"
#include "dbug.h"
#include "defs.h"
#include "util.h"

int test1_ok = 0;
int test2_ok = 0;
int test3_ok = 0;
int proc3_done = 0;

/* *****************************************************
 * TEST CASE 1: Testing send/receive message and request
 * memory block.
 * *****************************************************/
void test1()
{
	rtx_dbug_outs((CHAR *)"S11G022_test: START\r\n");
	rtx_dbug_outs((CHAR *)"S11G022_test: total 3 tests\r\n");

	void *mem_block = g_test_fixture.request_memory_block();
	*(char *)(mem_block + 64) = 'a';
	g_test_fixture.send_message(2, mem_block);

	/* Block process */
	g_test_fixture.receive_message(NULL);
	
	while(1) 
	{
		/* Should never enter here */
		g_test_fixture.release_processor();
	}
}

void test2()
{
	int sender_ID = -1;	
	void *envelope = g_test_fixture.receive_message(&sender_ID);
	char message = *(char *)(envelope + 64);

	if (sender_ID == 1 && message == 'a')
	{
		test1_ok = 1;
	}
	
	g_test_fixture.release_memory_block (envelope);

	/* Block process */
	g_test_fixture.receive_message(NULL);

	while(1) 
	{
		/* Should never enter here */
		g_test_fixture.release_processor();
	}
}


/* *****************************************************
 * TEST CASE 2: Testing delayed send, set/get process 
 * priority and preemption.
 * *****************************************************/
void test3()
{
	g_test_fixture.set_process_priority(3, 1);

	if (g_test_fixture.get_process_priority(3) == 1)
	{
		void *mem_block = g_test_fixture.request_memory_block();
		*(char *)(mem_block + 64) = 'b';
		g_test_fixture.delayed_send(3, mem_block, 5000);

		int sender_ID = -1;
		/* Block this process for the next 5 secs */
		void *envelope = g_test_fixture.receive_message(&sender_ID);
		rtx_dbug_outs(" Things work ");
		proc3_done = 1;
		if (sender_ID == 3 && *(char *)(envelope + 64) == 'b')
		{
			g_test_fixture.set_process_priority(3, 3);
			if (g_test_fixture.get_process_priority(3) == 3)
			{
				test2_ok = 1;
			}
		}
		g_test_fixture.release_memory_block (envelope);
	}

	/* Block process */
	g_test_fixture.receive_message(NULL);

	while(1) 
	{
		/* Should never enter here */
		g_test_fixture.release_processor();
	}
}

int add_one(int num)
{
	int result = num + 1;
	//g_test_fixture.release_processor();
	return result;
}

/* *****************************************************
 * TEST CASE 3: Testing proper stack management through
 * variables and function calls.
 * *****************************************************/
void test4()
{
	int i = 0;
	int n = 5;

	g_test_fixture.set_process_priority(4, 2);

	if (g_test_fixture.get_process_priority(4) == 2)
	{
		while (i < n) {
			i = add_one(i);
			}
	}

	if (i == n)
		test3_ok = 1;
	
	g_test_fixture.set_process_priority(4, 3);	

	/* Block process */
	g_test_fixture.receive_message(NULL);

	while(1) 
	{
		/* Should never enter here */
		g_test_fixture.release_processor(NULL);
	}
}

void test5()
{
	while(!proc3_done)
	{
		g_test_fixture.release_processor();
	}

	void *mem_block = g_test_fixture.request_memory_block();
	g_test_fixture.send_message(6, mem_block);
	g_test_fixture.release_memory_block (mem_block);

	while (1)
	{
		g_test_fixture.release_processor();
	}
}


/* *****************************************************
 * Output results to janusROM terminal
 * *****************************************************/
void test6()
{
	/* Block process 6 until other processes finish running */
	void *mem_block = g_test_fixture.receive_message(NULL);
	g_test_fixture.release_memory_block (mem_block);

	if (test1_ok)
		rtx_dbug_outs((CHAR *)"S11G022_test: test 1 OK\r\n");
	else
		rtx_dbug_outs((CHAR *)"S11G022_test: test 1 FAIL\r\n");

	if (test2_ok)
		rtx_dbug_outs((CHAR *)"S11G022_test: test 2 OK\r\n");
	else
		rtx_dbug_outs((CHAR *)"S11G022_test: test 2 FAIL\r\n");

	if (test3_ok)
		rtx_dbug_outs((CHAR *)"S11G022_test: test 3 OK\r\n");
	else
		rtx_dbug_outs((CHAR *)"S11G022_test: test 3 FAIL\r\n");

	int num_tests_ok = test1_ok + test2_ok + test3_ok;
	int num_tests_fail = 3 - num_tests_ok;

	rtx_dbug_outs((CHAR *)"S11G022_test: ");
	if ( num_tests_ok == 0 ) {
		rtx_dbug_outs((CHAR *)"0");
		}
	if ( num_tests_ok == 1 ) {
		rtx_dbug_outs((CHAR *)"1");
		}	
	if ( num_tests_ok == 2 ) {
		rtx_dbug_outs((CHAR *)"2");
		}
	if ( num_tests_ok == 3 ) {
		rtx_dbug_outs((CHAR *)"3");
		}		
	rtx_dbug_outs((CHAR *)"/3 tests OK\r\n");

	rtx_dbug_outs((CHAR *)"S11G022_test: ");
	if ( num_tests_fail == 0 ) {
		rtx_dbug_outs((CHAR *)"0");
		}
	if ( num_tests_fail == 1 ) {
		rtx_dbug_outs((CHAR *)"1");
		}	
	if ( num_tests_fail == 2 ) {
		rtx_dbug_outs((CHAR *)"2");
		}
	if ( num_tests_fail == 3 ) {
		rtx_dbug_outs((CHAR *)"3");
		}
	rtx_dbug_outs((CHAR *)"/3 tests FAIL\r\n");
	
	rtx_dbug_outs((CHAR *)"S11G022_test: END\r\n");

	while(1) {
		g_test_fixture.release_processor();
	}
}

/* register the third party test processes with RTX */
void __attribute__ ((section ("__REGISTER_TEST_PROCS__")))register_test_proc()
{
    int i;
	 test1_ok = 0;
	 test2_ok = 0;
	 test3_ok = 0;
	 proc3_done = 0;

    ////////rtx_dbug_outs((CHAR *)"rtx_test: register_test_proc()\r\n");

    for (i =0; i< NUM_TEST_PROCS; i++ ) {
        g_test_proc[i].pid = i + 1;
        g_test_proc[i].priority = 3;
        g_test_proc[i].sz_stack = 512;
    }
    g_test_proc[0].entry = test1;
    g_test_proc[1].entry = test2;
    g_test_proc[2].entry = test3;
    g_test_proc[3].entry = test4;
    g_test_proc[4].entry = test5;
    g_test_proc[5].entry = test6;
}

/**
 * Main entry point for this program.
 * never get invoked
 */
int main(void)
{
    ////////rtx_dbug_outs((CHAR *)"rtx_test: started\r\n");
    return 0;
}
