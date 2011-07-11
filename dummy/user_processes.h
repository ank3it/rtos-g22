#ifndef _USER_PROCESSES_H_
#define _USER_PROCESSES_H_

#include "envelope.h"

extern void ProcessA();
extern void ProcessB();
extern void ProcessC();
extern void enqueue_local_message(struct envelope *message);


typedef struct temp_msg_queue
{
	struct envelope *message;
	struct tmp_msg_queue *next;
};

struct temp_msg_queue *temp_message_head;
struct temp_msg_queue *temp_message_tail;

#endif