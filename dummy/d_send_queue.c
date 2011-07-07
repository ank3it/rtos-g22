/*
 *  d_send_queue.c
 *  
 *
 *  Created by Ankit Srivastava on 11-07-07.
 *  Copyright 2011 University of Waterloo. All rights reserved.
 *
 */

#include "d_send_queue.h"
#include "timer.h"

extern SINT32 __Counter2;
/**
 * @brief: Initialize the queue 
 * @param: q The queue to operate on
 */
void queue_init(struct queue *q)
{
	q->head = q->tail = NULL;	
}

/**
 * @brief: Add node to queue according to priority
 * @param: q The queue to operate on
 * @param: value The value to be added
 * @param: priority The priority of the node
 * @return: int Returns 0 on success, -1 otherwise
 */
int enqueue(struct queue *q, int process_ID, void * MessageEnvelope, int delay)
{
	if (q == NULL)
		return RTX_ERROR;
	
	struct queue_node *new_node = malloc(sizeof(struct queue_node));
	new_node->MessageEnvelope = MessageEnvelope;
	new_node->process_ID = process_ID;
	new_node->delay = delay;
	
	if (q->head == NULL)
	{
		/* Inserting in empty queue */
		q->head = q->tail = new_node;
		new_node->next = NULL;
	}
	else	
	{
		struct queue_node *curr_node = q->head;
		
		if (delay < curr_node->delay)
		{
			/* Inserting at the head of the queue */
			new_node->next = q->head;
			q->head = new_node;
		} 
		else
		{
			/* Inserting after head */
			
			/* Traverse queue to proper insert location */
			while (delay >= curr_node->next->delay
				   && curr_node->next != NULL)
				curr_node = curr_node->next;
			
			/* Inserting at the tail */
			if (curr_node == q->tail)
				q->tail = new_node;
			
			new_node->next = curr_node->next;
			curr_node->next = new_node;
		}
	}
	
	return RTX_SUCCESS;
}

/**
 * @brief: Remove the head node and return its value
 * @param: q The queue to operate on
 * @return: int Returns the value of the dequeued node if successful
 * 		else returns -1
 */
int dequeue(struct queue *q) //how do i return a message envelope type
{
	int return_value;
	if (q == NULL || q->head == NULL)
		return RTX_ERROR;
	
	struct queue_node *dequeue_node = q->head;
	
	if( __Counter2 < dequeue_node->delay){
		
		return_value = dequeue_node->MessageEnvelope;
		
		if (q->head == q->tail)
			q->head = q->tail = NULL;
		else
			q->head = q->head->next;
		
		free(dequeue_node);
		
		return return_value;
	
	}else {
				//what should i be doing here
	}

}

