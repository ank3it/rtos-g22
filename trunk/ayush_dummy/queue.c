/**
 * @group: S11-G22
 * @file: queue.c
 * @date: 2011/05/30
 * @brief: Implementation of a priority queue
 */

#include "queue.h"

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
int enqueue(struct queue *q, int value, int priority)
{
	if (q == NULL)
		return RTX_ERROR;

	struct queue_node *new_node = malloc(sizeof(struct queue_node));
	new_node->value = value;
	new_node->priority = priority;

	if (q->head == NULL)
	{
		/* Inserting in empty queue */
		q->head = q->tail = new_node;
		new_node->next = NULL;
	}
	else	
	{
		struct queue_node *curr_node = q->head;

		if (priority < curr_node->priority)
		{
			/* Inserting at the head of the queue */
			new_node->next = q->head;
			q->head = new_node;
		} 
		else
		{
			/* Inserting after head */
			
			/* Traverse queue to proper insert location */
			while (priority >= curr_node->next->priority
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
int dequeue(struct queue *q)
{
	if (q == NULL || q->head == NULL)
		return RTX_ERROR;

	struct queue_node *dequeue_node = q->head;
	int return_value = dequeue_node->value;

	if (q->head == q->tail)
		q->head = q->tail = NULL;
	else
		q->head = q->head->next;

	free(dequeue_node);

	return return_value;
}

/**
 * @brief: Remove the given value from the queue
 * @param: q The queue to operate on
 * @param: value The value of the node to remove
 * @return: Returns 0 on success, -1 otherwise
 */
int remove(struct queue *q, int value)
{
	if (q == NULL || q->head == NULL)
		return RTX_ERROR;

	struct queue_node *node = q->head;

	/* Search for the node */
	if (node->value == value)
	{
		/* Removing the head node */
		dequeue(q);

		return RTX_SUCCESS;
	}
	else
	{
		/* Search the rest of the queue */
		while (node->next != NULL)
		{
			if (node->next->value == value)
			{
				struct queue_node *remove_node = node->next;

				if (node->next == q->tail)
				{
					node->next = NULL;
					q->tail = node;
				}
				else
					node->next = remove_node->next;

				free(remove_node);
				return RTX_SUCCESS;
			}

			node = node->next;
		}
		return RTX_ERROR;
	}
}
