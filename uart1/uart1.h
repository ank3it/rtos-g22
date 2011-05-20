/*
 * @group: S11-G22
 * @file: uart1.h
 * @date: 2011/05/11
 * @brief: Header file for uart1.c
 */

#ifndef _UART1_H_
#define _UART1_H_

#include "../shared/rtx_inc.h"
#include "../dbug/dbug.h"

volatile BOOLEAN Caught;
volatile BYTE CharIn;
volatile BYTE CharOut;
volatile BOOLEAN TransmitReady;

VOID uart1_init();				/* Initialize uart1 */
VOID c_serial_handler( VOID );	/* Interrupt handler for UART1 */
VOID print_char( CHAR c);		/* Put the character in the transmit register */
VOID print_string( CHAR * s );	/* Print given string to UART1 */

#endif /* _UART1_H_ */
