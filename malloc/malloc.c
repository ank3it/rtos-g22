/**
 * @group: S11-G22
 * @file: malloc.c
 * @date: 2011/06/04
 * @brief: Implementation of a very simple memory allocator
 * 	for allocating kernel memory only.
 */

#include "malloc.h"

void *free_memory_start;
void *last_valid_address;

/**
 * @brief: Initialize malloc
 * @param: start_ptr The start of the free memory region
 */
void malloc_init(void *starting_address)
{
	free_memory_start = last_valid_address = starting_address;
}

/**
 * @brief: Allocate a block of memory greater than or equal to
 *	the given size. Preference is given to memory blocks that 
 *	exactly match the given size
 * @param: size The size of the memory block to allocate
 * @return: Pointer to the memory block, NULL on failure
 */
void * malloc(int size)
{
	/* Check if size is valid */
	if (size < 1)
		return NULL;

	void *memory_location;
	void *current_location;
	struct memory_control_block *mcb;

	/* Adjust size to include the memory control block */
	size += sizeof(struct memory_control_block);

	memory_location = NULL;

	/* Begin searching for available space from free_memory_start */
	current_location = free_memory_start;

	while (current_location != last_valid_address)
	{
		mcb = (struct memory_control_block *)current_location;

		if (mcb->is_available && mcb->size >= size)
		{
			/* Found available memory! */
			memory_location = current_location;
			
			if (mcb->size == size)
				break;
		}

		/* Move to next memory block */
		current_location += mcb->size;
	}

	if (memory_location != NULL)
	{
		mcb = (struct memory_control_block *)memory_location;
		mcb->is_available = FALSE;

		memory_location += sizeof(struct memory_control_block);
	}
	else
	{
		/* If we still haven't found a suitable memory block */
		memory_location = last_valid_address;

		last_valid_address += size;

		mcb = memory_location;
		mcb->is_available = FALSE;
		mcb->size = size;

		/* Move past MCB and point to actual free memory */
		memory_location +=  sizeof(struct memory_control_block);
	}

	return memory_location;
}

/**
 * @brief: Free the memory at the given address
 * @param: memory_location The memory address being freed
 */
void free(void *memory_location)
{
	if (memory_location == NULL)
		return;
	
	struct memory_control_block *mcb;

	/* Backup to get the MCB of the memory location */
	memory_location -= sizeof(struct memory_control_block);

	if (memory_location < free_memory_start
		|| memory_location > last_valid_address)
	{
		return;
	}

	mcb = (struct memory_control_block *)memory_location;
	mcb->is_available = TRUE;

	return;
}
