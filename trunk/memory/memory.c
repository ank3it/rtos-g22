/**
 * @author: 
 * @brief: ECE 354 S11 RTX Project P1-(c) 
 * @date: 2011/05/02
 */

#include "../shared/rtx_inc.h"
#include "../dbug/dbug.h"
#include "memory.h"

#define NUM_MEM_BLKS 32

extern int __end;
int* head;
int* tail;

CHAR* itoa( int i ) {
	int count = 0;
	CHAR* out = "";
	CHAR out_rev = "";
	CHAR out_rev2 = "";
	CHAR* out_track = out;
	CHAR* out_track2 = out;
	int val = 0;
	
	while( i != 0 ) {
		val = i % 10;
		val = val + 48;
		*out = (CHAR*)val;
		*out++;
		i = i / 10;
		count++;
	}
	
	count = count / 2;
	
	*out = '\0';
	*out--;
	
	while( count != 0 ) {
		out_rev = *out;
		out_rev2 = *out_track;
		*out_track = out_rev;
		*out = out_rev2;
		*out--;
		*out_track++;
		count--;
	}
	
	return out_track2;
}

/**
 * @brief: Memory initialization routine
 */
void init_memory()
{
	int i = 0 ;
	head = &(__end);
	head++;
	tail = head;
	rtx_dbug_outs( (CHAR *) "\n\r Head = " );
	rtx_dbug_outs( itoa(head) );
	rtx_dbug_outs( (CHAR *) "\n\r Head Pointer  = " );
	rtx_dbug_outs( itoa(*head) );
	while ( i< NUM_MEM_BLKS ) {
		// NUM_MEM_BLKS - i - Offst of list length
		tail++;
		*tail = head + ( NUM_MEM_BLKS + 1 ) + ( 32 * i ); /* Using 32 * i as we can store 128/4 = 32 integers */
		rtx_dbug_outs( (CHAR *) " Tail = " );
		rtx_dbug_outs( itoa(tail) );
		rtx_dbug_outs( (CHAR *) "\n\r Tail Pointer = " );
		rtx_dbug_outs( itoa(*tail) );
		i++;
	}
	
    /* Add your own code here */
}

/**
 * @brief request a free memory block of size 128 Bytes
 * @return starting address of a free memory block
 *         and NULL on error 
 */

void* s_request_memory_block()
{
	void* returnVal = *tail;
	if ( tail == head ) {
		return NULL;
	}
	
	tail--;
	rtx_dbug_outs( (CHAR *) "\n\r Address Send = " );
	rtx_dbug_outs( itoa(returnVal) );
	return returnVal;
	
}
	

/**
 * @param: address of a memory block to free
 * @return: 0 on sucess, non-zero on error
 */

int s_release_memory_block( void* memory_block )
{
	int* temp_tail;
	int traverse_mem;
	int chk_mem_blk = memory_block;
	int error = 1;
	int i = 0;
	void* returnVal;
	
	
	rtx_dbug_outs( (CHAR *) "\n\r Address Received = " );
	rtx_dbug_outs( (CHAR *) "  " );
	rtx_dbug_outs( itoa(memory_block) );
	
	// Check if we are freeing only the assigned blocks
	rtx_dbug_outs( (CHAR *) "\n\r Check Assigned Blocks - Traverse Memory" );
	while ( i< NUM_MEM_BLKS ) {
		traverse_mem = (int)head + ( NUM_MEM_BLKS + 1 )*4 + ( 32 * i )*4; 
		rtx_dbug_outs( (CHAR *) "  " );
		rtx_dbug_outs( itoa(traverse_mem) );
		if ( chk_mem_blk == traverse_mem ) {
			error = 0;
			break;
		}
		i++;
	}
	
	if ( error == 1 ) {
		return -1;
	}
		
	// Check if we are freeing already freed mem
	temp_tail = tail;
	rtx_dbug_outs( (CHAR *) "\n\r Temp_Tail = " );
	rtx_dbug_outs( itoa(temp_tail) );
	rtx_dbug_outs( (CHAR *) "\n\r Checking Free Mem - ReturnVal " );
	while ( temp_tail != head ) {
		returnVal = *temp_tail;
		temp_tail--;
		rtx_dbug_outs( (CHAR *) "  " );
		rtx_dbug_outs( itoa(returnVal) );
		if( memory_block == returnVal ) {
			error = 1;
		}
	}
	
	if ( error == 1 ) {
		return -1;
	}

	rtx_dbug_outs( (CHAR *) "\n\r Tail = " );
	rtx_dbug_outs( itoa(tail) );
	
	// Update Tail
	tail++;
	*tail = memory_block;
	
	rtx_dbug_outs( (CHAR *) "\n\r Tail = " );
	rtx_dbug_outs( itoa(tail) );
	rtx_dbug_outs( (CHAR *) "\n\r Tail Pointer = " );
	rtx_dbug_outs( itoa(*tail) );
	return 0;

	
    /* add code to return non-zero if releasing memory block fails */
}
