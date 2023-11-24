/*
 * Terminal_stdio.h
 *
 *  Created on: 2023. 11. 24.
 *      Author: 현대오토9
 */

#ifndef MYLIB_TERMINAL_STDIO_H_
#define MYLIB_TERMINAL_STDIO_H_

extern void Init_Mystdio(void);
extern void my_puts(const char *str);
extern int my_printf(const char *format, ...);
extern void my_scanf(const char *fmt, ...);

#endif /* MYLIB_TERMINAL_STDIO_H_ */
