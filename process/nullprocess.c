#include "nullprocess.h"

void null_process()
{
	while(TRUE)
	{
		rtx_dbug_out_char("------Null Process------\n\r");
		release_processor();
	}
}