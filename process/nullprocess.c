#include "nullprocess.h"

void null_process()
{
	while(1)
	{
		rtx_dbug_out_char((CHAR *) "------Null Process------\n\r");
		k_release_processor();
	}
}
