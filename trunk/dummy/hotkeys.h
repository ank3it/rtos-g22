/**
 * @group: S11-G22
 * @file: hotkeys.h
 * @date: 2011/07/02
 * @brief: Hotkeys header
 */

#ifndef _HOTKEYS_H_
#define _HOTKEYS_H_

#include "process.h"
#include "envelope.h"
#include "rtx_inc.h"
#include "dbug.h"
#include "util.h"
#include "malloc.h"
#include "defs.h"

void print_ready_queue();
void print_blocked_memory_queue();
void print_blocked_receive_queue();
void print_avail_memory_block_queue();
void print_all_process_info();
void print_message_queue();

#endif /* _HOTKEYS_H_ */