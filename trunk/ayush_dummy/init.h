#ifndef _INIT_H_
#define _INIT_H_

#include "process.h"
#include "defs.h"
#include "memory.h"
#include "timer.h"
#include "queue.h"

void load_null_process();
void load_uart_iprocess();
void load_crt_process();
void load_kcd_process();
void load_test_processes();
void init_pcb();
void null_process();
void init_funcs();

#endif /* _INIT_H_ */
