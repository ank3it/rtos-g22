/*
 *  d_send_queue.h
 *  
 *
 *  Created by Ankit Srivastava on 11-07-07.
 *  Copyright 2011 University of Waterloo. All rights reserved.
 *
 */

#ifndef _QUEUE_H_D
#define _QUEUE_H_D

#include "rtx_inc.h"
#include "dbug.h"
#include "util.h"
#include "malloc.h"

struct d_queue_node
{
	int process_ID;
	void * MessageEnvelope; 
	int delay;
	int sender_id;
	struct d_queue_node *next;
};

struct d_queue
{
	struct d_queue_node *head;
	struct d_queue_node *tail;
};

/* Initialize the queue */
void d_queue_init(struct d_queue *);

/* Add node to queue */
int d_enqueue(struct d_queue *, int, void *, int , int);

int d_dequeue(struct d_queue *q);

#endif /* _QUEUE_H_ */
