#ifndef _INIT_H_
#define _INIT_H_


#include "process.h"
#include "defs.h"
#include "rtx_test.h"
#include "nullprocess.h"
#include "memory.h"
#include "timer.h"

void load_null_process();
void load_test_processes();
void init_pcb();
process* get_proc(int);

#endif /* _INIT_H_ */