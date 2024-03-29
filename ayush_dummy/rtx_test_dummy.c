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

CHAR* itoa(int n)
{
	int i, sign;
	int num_digits = 1;

	/* Get number of digits in n */
	i = n;
	do
	{
		i /= 10;
		++num_digits;
	} while (i != 0);

	/* Record sign, make n positive, increase num_digits */
	if ((sign = n) < 0)						
	{		
		n = -n;
		++num_digits;
	}

	/* CHAR array to hold the result */
	CHAR s[num_digits];

	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	if (sign < 0)
		s[i++] = '-';				/* Append negative sign */

	s[i] = '\0';					/* Append terminating null char */

	reverse(s, num_digits);
	
	return s;
}

/**
 * @brief: Reverses the given string
 * @param: s String to be reversed
 * @param: length The length of the string
 */
VOID reverse(CHAR *s, int length)
{
	CHAR c;
	int i = 0;
	int j = length - 2;

	while (i < j)	
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;

		i++;
		j--;
	}
}

/* third party dummy test process 1 */ 
void test1()
{
	while(1) {
		TRACE("\r\n--------------------\r\n");
		TRACE("TEST 1\r\n");
		TRACE("--------------------\r\n");

		int sender_ID = -1;
		TRACE("Calling receive_message()\r\n");
		void * envelope = g_test_fixture.receive_message(&sender_ID);
		TRACE("sender_ID = ");
		TRACE(itoa(sender_ID));
		TRACE("\r\n");
		TRACE("envelope = ");
		TRACE(itoa(envelope));
		TRACE("\r\nvalue at message = ");
		TRACE(itoa(*(int *)(envelope + 64)));
		TRACE("\r\n");

		TRACE("Calling receive_message()\r\n");
		void * envelope2 = g_test_fixture.receive_message(&sender_ID);
		TRACE("sender_ID = ");
		TRACE(itoa(sender_ID));
		TRACE("\r\n");
		g_test_fixture.release_processor();
	}
}

/* third party dummy test process 2 */ 
void test2()
{
	while(1) {
		TRACE("\r\n--------------------\r\n");
		TRACE("TEST 2\r\n");
		TRACE("--------------------\r\n");

		TRACE("Calling request_memory_block()\r\n");
		void *envelope = g_test_fixture.request_memory_block();
		TRACE("evelope = ");
		TRACE(itoa(envelope));
		int *num = (int *)(envelope + 64);
		*num = 555;
		TRACE("\r\nnum = ");
		TRACE(itoa(*num));
		TRACE("\r\nCalling send_message()\r\n");
		g_test_fixture.delayed_send(1, envelope , 3);
		g_test_fixture.set_process_priority(1, 1);
		
		TRACE("\r\nCalling request_memory_block()\r\n");
		void *envelope2 = g_test_fixture.request_memory_block();
		*(int *)(envelope2 + 64) = 123;
		TRACE("Calling send_message()\r\n");
		g_test_fixture.send_message(1, envelope2);

		g_test_fixture.release_processor();
	}
}
/* third party dummy test process 3 */ 
void test3()
{
	while(1) {
		TRACE("\r\n--------------------\r\n");
		TRACE("TEST 3\r\n");
		TRACE("--------------------\r\n");
		g_test_fixture.release_processor();
		rtx_dbug_outs((CHAR *)"blah3\r\n");
	}
}

/* third party dummy test process 4 */ 
void test4()
{
	while(1) {
		TRACE("\r\n--------------------\r\n");
		TRACE("TEST 4\r\n");
		TRACE("--------------------\r\n");
		g_test_fixture.release_processor();
		rtx_dbug_outs((CHAR *)"blah4\r\n");
	}
}
/* third party dummy test process 5 */ 
void test5()
{
	while(1) {
		TRACE("\r\n--------------------\r\n");
		TRACE("TEST 5\r\n");
		TRACE("--------------------\r\n");
		g_test_fixture.release_processor();
		rtx_dbug_outs((CHAR *)"blah5\r\n");
	}
}
/* third party dummy test process 6 */ 
void test6()
{
	while(1) {
		TRACE("\r\n--------------------\r\n");
		TRACE("TEST 6\r\n");
		TRACE("--------------------\r\n");
		g_test_fixture.release_processor();
		rtx_dbug_outs((CHAR *)"blah6\r\n");
	}
}

/* register the third party test processes with RTX */
void __attribute__ ((section ("__REGISTER_TEST_PROCS__")))register_test_proc()
{
    int i;

    rtx_dbug_outs((CHAR *)"rtx_test: register_test_proc()\r\n");

    for (i =0; i< NUM_TEST_PROCS; i++ ) {
        g_test_proc[i].pid = i + 1;
        g_test_proc[i].priority = 3;
        g_test_proc[i].sz_stack = 2048;
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
    rtx_dbug_outs((CHAR *)"rtx_test: started\r\n");
    return 0;
}
