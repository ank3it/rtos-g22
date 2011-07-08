/**
 * @group: S11-G22
 * @file: uart.h
 * @date: 2011/06/27
 * @brief: Header file for uart.c
 */

#ifndef _UART_H_
#define _UART_H_

/* Struct representing a command registration */
struct keyboard_command
{
	char command_identifier[MAX_IDENTIFIER_LENGTH];
	int registrant_process_ID;
	struct keyboard_command *next;
};

/* Initialize hotkeys */
void hotkeys();

/* Initialize uart */
void uart_init();

/* Interrupt handler for UART1 */
void c_uart_handler();

/* Process that gets loaded by the uart ISR */
void uart_iprocess();

/* Keyboard command decoder responsible for keyboard 
 * input and command registration */
void kcd_process();

/* CRT display process responsible for displaying text to screen */
void crt_display_process();

#endif /* _UART_H_ */
