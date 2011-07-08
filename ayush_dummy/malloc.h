/**
 * @group: S11-G22
 * @file: malloc.h
 * @date: 2011/06/04
 * @brief: Header file for malloc.c
 */

#ifndef _MALLOC_H_
#define _MALLOC_H_

#include "rtx_inc.h"
#include "dbug.h"
#include "util.h"

struct memory_control_block
{
	int is_available;
	UINT16 size;
};

/* Initialize malloc */
void malloc_init(void *starting_address);

/* Allocate a block of memory of the given size */
void * malloc(int size);

/* Free the memory at the given address */
void free(void *memory_location); 

#endif /* _MALLOC_H_ */
