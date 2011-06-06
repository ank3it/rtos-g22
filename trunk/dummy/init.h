#ifndef _INIT_H_
#define _INIT_H_

#include "process.h"
#include "defs.h"
#include "nullprocess.h"
#include "../memory/memory.h"
#include "../timer0/timer.h"
#include "queue.h"

void load_null_process();
void load_test_processes();
void init_pcb();
void null_process();
void init_funcs();

#endif /* _INIT_H_ */
