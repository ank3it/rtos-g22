/**
 * @file: dbug.h
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang  
 * @date: 2010/05/03
 * @brief: Header file of dbug.c.  
 */
#ifndef _DBUG_H_
#define _DBUG_H_

#include "rtx_inc.h"

VOID rtx_dbug_out_char( CHAR c );   // output a char to janusROM terminal
SINT32 rtx_dbug_outs( CHAR* s );    // output a string to janusROM terminal

/* TRACE macro for debug statements */
#ifdef _DEBUG_
#define TRACE(MSG) 						\
do { rtx_dbug_outs((CHAR *) MSG); }		\
while (0)
#else
#define TRACE(MSG) do { ; } while (0)
#endif

#endif /* _DBUG_H_ */
