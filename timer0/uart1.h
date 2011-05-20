/*
 * @group: S11-G022
 * @file: uart1.h
 * @date: 2011/05/11
 * @brief: Header file for uart1.c
 */

#ifndef _UART1_H_
#define _UART1_H_

#include "rtx_inc.h"

VOID uart1_init();				// Initialize uart1
VOID print_char( CHAR c );		// Output a char to UART1
VOID print_string( CHAR * s );	// Ouput a string to UART1

#endif /* _UART1_H_ */
