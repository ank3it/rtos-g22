#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "process.h"
#include "../dbug/dbug.h"
#include "init.h"

void save_context(int PID);
void load_context(int PID);

#endif /* _CONTEXT_H_ */
