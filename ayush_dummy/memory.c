/**
 * @author: 
 * @brief: ECE 354 S11 RTX Project P1-(c) 
 * @date: 2011/05/02
 */

#include "rtx_inc.h"
#include "dbug.h"
#include "memory.h"
#include "defs.h"
#include "malloc.h"
#include "process.h"

//extern int __end;
int* head;
int* tail;

/**
 * @brief: Memory initialization routine
 */
void init_memory()
{
	int i = 0 ;
	head = malloc( (NUM_MEM_BLKS + 1) * MAX_BLK_SIZE + (NUM_MEM_BLKS + 2 ) * sizeof(int*) )  ;
	head++;
	tail = head;
		while ( i < NUM_MEM_BLKS ) {
		// NUM_MEM_BLKS - i - Offst of list length
		tail++;
		*tail = head + ( NUM_MEM_BLKS + 1 ) + ( MAX_BLK_SIZE / 4 * i ); /* Using 32 * i as we can store 128/4 = 32 integers */
		i++;
	}
	
    /* Add your own code here */
}

/**
 * @brief request a free memory block of size 128 Bytes
 * @return starting address of a free memory block
 *         and NULL on error 
 */

void* k_request_memory_block()
{
	TRACE("k_request_memory_block()\r\n");
	void* returnVal = *tail;
	if ( tail == head ) {
		block_running_process(BLOCK_MEMORY);
		return 0;
	}
	
	tail--;
	TRACE("returnVal = ");
	TRACE(itoa(returnVal));
	TRACE("\r\n");
	return returnVal;
}
	

/**
 * @param: address of a memory block to free
 * @return: 0 on sucess, non-zero on error
 */

int k_release_memory_block( void* memory_block )
{
	TRACE("k_release_memory_block()\r\n");
	TRACE("memory_block = ");
	TRACE(itoa(memory_block));
	TRACE("\r\n");
	int* temp_tail;
	int traverse_mem;
	int chk_mem_blk = memory_block;
	int error = 1;
	int i = 0;
	void* returnVal;
	
	// Check if we are freeing only the assigned blocks
	TRACE("Check Assigned Blocks - Traverse Memory\r\n");
	while ( i< NUM_MEM_BLKS ) {
		traverse_mem = (int)head + ( NUM_MEM_BLKS + 1 )*4 + ( MAX_BLK_SIZE / 4 * i )*4; 
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
	while ( temp_tail != head ) {
		returnVal = *temp_tail;
		temp_tail--;
		if( memory_block == returnVal ) {
			error = 1;
		}
	}
	
	if ( error == 1 ) {
		return -1;
	}

	// Update Tail
	tail++;
	*tail = memory_block;
	
	unblock_process(NULL, BLOCK_MEMORY);
	
	return 0;

	
    /* add code to return non-zero if releasing memory block fails */
}
