/**
 * @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>
 *
 * Useful debug utilities for serial port based output.
 */

#pragma once
#include "util.h"

/**
 * Prints a string via serial port.
 *
 * @param s pointer to a string
 */
extern void s_puts(char *s);

/**
 * Prints a single character via serial port.
 *
 * @ param c character
 */
extern void s_putc(char c);

/**
 * Initializes serial port.
 */
extern void s_start(void);

/**
 * Prints a single byte in hexadecimal.
 *
 * @param b unsigned byte
 */
void put_byte(u8 b);

/**
 * Prints a 32bit wide value in hexadecimal.
 *
 * @param n unsigned 32bit value
 */
void put_hex(u32 n);

/**
 * libc-like print formatting. Supports the following substitutions:
 *
 * %s -> char * strings
 * %x -> u32    hexadecimal
 * %% -> %      actual '%'
 *
 * @param f format string
 * @param ... args
 */
void printf(char *f, ...);
