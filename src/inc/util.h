/**
 * @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>
 *
 * General utilities shared across source files.
 */

#pragma once
#include <stddef.h>

/**
 * Reads w bytes from s address. Ex: IN(u16, 0x10000)
 */
#define IN(w, s) *((w *) (s))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

/**
 * Read count sectors from drive to address at seg:off.
 *
 * @param sector lba sector index
 * @param count number of sectors to read
 * @param seg load address segment
 * @param off load address offset 
 */
void lba_read(u32 sector, u16 count, u16 seg, u16 off);

/**
 * Copy n bytes from src to destination.
 *
 * @param *dest destination address
 * @param *src source address
 * @param n number of bytes to copy
 */
void memcpy(u8 *dest, const u8 *src, size_t n);

/**
 * Read count sectors from drive starting at sector.
 *
 * @param sector lba sector index
 * @param count number of sectors to read
 */
void read_sectors(u32 sector, u16 count);

/**
 * Compare two memory locations up util n bytes. Returns zero if all bytes are equal.
 *
 * @param *a pointer to first memeory location
 * @param *b pointer to second memeory location
 * @param n number of bytes to compare
 * @return zero if locations are equal
 */
size_t strncmp(const u8 *a, const u8 *b, size_t n);

/**
 * Return the length of a string. Terminates upon finding a null byte.
 *
 * @param *s pointer to string
 * @return length of string in bytes
 */
size_t strlen(const u8 *s);
