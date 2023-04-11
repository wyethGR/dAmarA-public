/**
 * @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>
 */

#include "inc/config.h"
#include "inc/util.h"

extern void lba_read(u32 sector, u16 count, u16 seg, u16 off);

void memcpy(u8 *dest, const u8 *src, size_t n) {
  for (size_t i = 0; i < n; i++) {
    *(dest++) = *(src++);
  }
}

void read_sectors(u32 sector, u16 count) {
  lba_read(sector, count, 0x0000, SEC_BUF_OFF);
}

size_t strncmp(const u8 *a, const u8 *b, size_t n) {
  while (n && *a && (*a == *b)) {
    a++;
    b++;
    n--;
  }

  return n;
}

size_t strlen(const u8 *s) {
  size_t n = 0;
  while (*s) {
    n++;
    s++;
  }

  return n;
}
