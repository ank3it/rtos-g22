/**
 * @group: S11-G22
 * @file: msg.h
 * @date: 2011/06/24
 * @brief: Header file for msg.c
 */

#ifndef _MSG_H_
#define _MSG_H_

#include "../shared/rtx_inc.h"
#include "../dbug/dbug.h"
#include "../util/util.h"
#include "../malloc/malloc.h"
#include "defs.h"
#include "queue.h"

/*
 * Global variables
 */
 
 //Defining message envelope 
struct MessageEnvelope
{
	void* k_ptr;
	int sender_proc_id;
	int dest_proc_id;
	int message_type;
};