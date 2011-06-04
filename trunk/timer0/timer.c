/**
 * @file: timer.c
 * @brief: timer 0 smaple code
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang
 * @date: 2011/01/04
 */

#include "../shared/rtx_inc.h"
#include "../uart1/uart1.h"
#include "timer.h"

#define MaxSec	86400

/*
 * Global Variables
 */
SINT32 Counter = 86390;


/*
 * gcc expects this function to exist
 */
int __main( void )
{
    return 0;
}

/*
 * This function is called by the assembly STUB function
 */
VOID c_timer_handler( VOID )
{
    TIMER0_TER = 2;								/* Acknowledge interrupt */ 

	int hours = 0 ;
	int mins = 0 ;
	int secs  = 0;
	CHAR timeStr[] = "00:00:00\r";
	
    Counter++;
	if ( Counter == MaxSec ) {
		Counter = 0;
	}
	
	secs = Counter % 60;
	mins = Counter / 60;
	hours = mins / 60;
	mins = mins % 60;
	
	timeStr[0] = ( hours / 10 ) % 10 + 48;
	timeStr[1] = hours % 10 + 48;
	
	timeStr[3] = ( mins / 10 ) % 10 + 48;
	timeStr[4] = mins % 10 + 48;
	
	timeStr[6] = ( secs / 10 ) % 10 + 48;
	timeStr[7] = secs % 10 + 48;
	
	asm( "move.w #0x2000,%sr" );				/* Re-Enable all interrupts */
	print_string( timeStr );					/* Print to RTX console */
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
void timer_init( void )
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
}
