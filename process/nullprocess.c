#include "nullprocess.h"

void null_process()
{
	while(1)
	{
		rtx_dbug_out_char("------Null Process------\n\r");
		release_processor();
	}
}