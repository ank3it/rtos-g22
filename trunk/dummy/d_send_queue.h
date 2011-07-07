/*
 *  d_send_queue.h
 *  
 *
 *  Created by Ankit Srivastava on 11-07-07.
 *  Copyright 2011 University of Waterloo. All rights reserved.
 *
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "rtx_inc.h"
#include "dbug.h"
#include "util.h"
#include "malloc.h"

struct queue_node
{
	int process_ID;
	void * MessageEnvelope; 
	int delay;
	struct queue_node *next;
};

struct queue
{
	struct queue_node *head;
	struct queue_node *tail;
};

/* Initialize the queue */
void queue_init(struct queue *);

/* Add node to queue */
int enqueue(struct queue *, int, void *, int);

/* Pop first node from queue and return node value */
int dequeue(struct queue *); 

/* Remove the node with the given process_ID from the queue */
int remove(struct queue *, int);

#endif /* _QUEUE_H_ */
