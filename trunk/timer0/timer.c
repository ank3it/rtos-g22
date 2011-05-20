/**
 * @file: timer.c
 * @brief: timer 0 smaple code
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang
 * @date: 2011/01/04
 */

#include "../shared/rtx_inc.h"
//#include "../dbug/dbug.h"
#include "../uart1/uart1.h"

/*
 * Global Variables
 */
SINT32 MaxSec = 86400;
SINT32 Counter = 86390;


/*
 * gcc expects this function to exist
 */
int __main( void )
{
    return 0;
}

CHAR* itoa( int i ) {
	int count = 0;
	CHAR* out = "";
	CHAR out_rev = "";
	CHAR out_rev2 = "";
	CHAR* out_track = out;
	CHAR* out_track2 = out;
	int val = 0;
	
	while( i != 0 ) {
		val = i % 10;
		val = val + 48;
		*out = (CHAR*)val;
		*out++;
		i = i / 10;
		count++;
	}
	
	count = count / 2;
	
	*out = '\0';
	*out--;
	
	while( count != 0 ) {
		out_rev = *out;
		out_rev2 = *out_track;
		*out_track = out_rev;
		*out = out_rev2;
		*out--;
		*out_track++;
		count--;
	}
	
	return out_track2;
}


/*
 * This function is called by the assembly STUB function
 */
VOID c_timer_handler( VOID )
{
	int hours = 0 ;
	int mins = 0 ;
	int secs  = 0;
	int print_num = 0;
	CHAR *out = "";
	
    Counter++;
	if ( Counter == MaxSec ) {
		Counter = 0;
	}
	
	secs = Counter % 60;
	mins = Counter / 60;
	hours = mins / 60;
	mins = mins % 60;
	
	print_string((CHAR *)( "\r" ) );
	
	print_num = (hours / 10)%10;
	out = (CHAR *)(print_num+48);
	print_char(out );
	print_num=0;
	print_num = hours % 10;
	out = (CHAR *)(print_num+48);
	print_char(out);
	
	print_string((CHAR *)":");
	
	print_num=0;print_num = (mins / 10)%10;
	out = (CHAR *)(print_num+48);print_char(out );
	print_num=0;print_num = mins % 10;
	out = (CHAR *)(print_num+48);print_char(out );
	
	print_string((CHAR *)":" );
	
	print_num=0;print_num = (secs / 10)%10;
	out = (CHAR *)(print_num+48);print_char(out);
	print_num=0;print_num = secs % 10;
	out = (CHAR *)(print_num+48);print_char(out);
	
	
	
    //rtx_dbug_out_char('.');
	
	
	    /*
     * Ack the interupt
     */
    TIMER0_TER = 2;
}




SINT32 coldfire_vbr_init( VOID )
{
    /*
     * Move the VBR into real memory
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
    UINT32 mask;

    /* Disable all interupts */
    asm( "move.w #0x2700,%sr" );

    coldfire_vbr_init();
	uart1_init();

    /*
     * Store the timer ISR at auto-vector #6
     */
    asm( "move.l #asm_timer_entry,%d0" );
    asm( "move.l %d0,0x10000078" );
	
	

    /*
     * Setup to use auto-vectored interupt level 6, priority 3
     */
    TIMER0_ICR = 0x9B;

    /*
     * Set the reference counts, ~10ms
     */
    TIMER0_TRR = 0x6DDD;

    /*
     * Setup the timer prescaler and stuff
     */
    TIMER0_TMR = 0x633D;

    /*
     * Set the interupt mask
     */
    mask = SIM_IMR;
    mask &= 0x0003fdff;
    SIM_IMR = mask;    

    /* Let the timer interrupt fire, lower running priority */
    asm( "move.w #0x2000,%sr" );

	print_string( ( CHAR * ) "This should print in the rtx console!" );
	
    /* Wait for 5 seconds to pass */
//    rtx_dbug_outs( (CHAR *) "Waiting approx. 5 seconds for Counter > 500\n\r" );
    for(;;){}
    return 0;
}




