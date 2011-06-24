/**
 * @group: S11-G22
 * @file: envelope.h
 * @date: 2011/06/22
 * @brief: Message envelope struct
 */

#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

struct envelope
{
	int sender_process_ID;
	int dest_process_ID;
	int message_type;
	struct envelope *next;
	void *message;
};

#endif /* _ENVELOPE_H_ */
