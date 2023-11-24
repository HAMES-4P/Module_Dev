/*
 * asclin.h
 *
 *  Created on: 2023. 11. 24.
 *      Author: 현대오토9
 */

#ifndef MYLIB_ASCLIN_H_
#define MYLIB_ASCLIN_H_

#include "Ifx_Console.h"

extern void _init_uart3(void);                 /* Initialization function   */
extern unsigned char _in_uart3(void);          /* Receive (and wait for) a character from the serial line */
extern int _poll_uart3(unsigned char *chr);
extern void _out_uart3(const unsigned char chr);

extern void _init_uart1(void);                 /* Initialization function   */
extern unsigned char _in_uart1(void);          /* Receive (and wait for) a character from the serial line */
extern int _poll_uart1(unsigned char *chr);
extern void _out_uart1(const unsigned char chr);

extern void _init_uart0(void);                 /* Initialization function   */
extern unsigned char _in_uart0(void);          /* Receive (and wait for) a character from the serial line */
extern int _poll_uart0(unsigned char *chr);
extern int _nonBlock_poll_uart0(unsigned char *chr);
extern void _out_uart0(const unsigned char chr);

#endif /* MYLIB_ASCLIN_H_ */
