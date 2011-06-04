#ifndef _INIT_H_
#define _INIT_H_

#include "process.h"
#include "defs.h"
#include "nullprocess.h"
#include "../memory/memory.h"
#include "../timer0/timer.h"

void load_null_process();
void load_test_processes();
void init_pcb();
struct process* get_proc(int);

#endif /* _INIT_H_ */