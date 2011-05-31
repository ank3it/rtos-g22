#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "process.h"
#include "dbug.h"

void save_context(INT PID);
void load_context(INT PID);

#endif /* _CONTEXT_H_ */