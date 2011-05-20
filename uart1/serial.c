/**
 * @group: S11-G22
 * @file: serial.c
 * @brief: UART1/second serial port/rtx terminal I/O sample code
 * @author: ECE Lab Instructors and TAs
 * @author: Irene Huang
 * @date: 2011/05/18
 */
#include "../shared/rtx_inc.h"
#include "../dbug/dbug.h"
#include "uart1.h"

/*
 * Global Variables
 */
CHAR StringHack[] = "You Typed a Q\n\r";

/*
 * gcc expects this function to exist
 */
int __main( void )
{
    return 0;
}

SINT32 coldfire_vbr_init( VOID )
{
    /*
     * Move the VBR into real memory
     *
     * DG: actually, it'll already be here.
     */
    asm( "move.l %a0, -(%a7)" );
    asm( "move.l #0x10000000, %a0 " );
    asm( "movec.l %a0, %vbr" );
    asm( "move.l (%a7)+, %a0" );
    
    return RTX_SUCCESS;
}

/*
 * Entry point, check with m68k-coff-nm
 */
int main( void )
{
    coldfire_vbr_init();
    
	uart1_init();
   
    rtx_dbug_outs((CHAR *) "Type Q or q on RTX terminal to quit.\n\r" );
    
    /* Busy Loop */
    while( CharIn != 'q' && CharIn != 'Q' )
    {
        if( !Caught )
        {
            Caught = TRUE;
			
			if ( CharIn == CR )
			{
				print_string( ( CHAR * ) "\r\n" );	
			}
			else
			{
            	CharOut = CharIn;
            
            	/* Nasty hack to get a dynamic string format, 
             	* grab the character before turning the interrupts back on. 
	             */
 	           StringHack[12] = CharIn;

	            /* enable tx interrupts  */
    	        SERIAL1_IMR = 3;

	    	    /* Now print the string to debug, 
            	 * note that interrupts are now back on. 
	             */
    	        rtx_dbug_outs( StringHack );
			}
        }
    }

    /* Disable all interupts */
    asm( "move.w #0x2700,%sr" );

    /* Reset globals so we can run again */
    CharIn = '\0';
    Caught = TRUE;
    return 0;
}
