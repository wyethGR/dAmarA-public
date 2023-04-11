/**
 * @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>
 */

#include <stdarg.h>
#include "inc/debug.h"

void put_byte(u8 b) {
  s_putc("0123456789ABCDEF"[(b & 0xF0) >> 4]);
  s_putc("0123456789ABCDEF"[b & 0x0F]);
  return;
}

void put_hex(u32 n) {
  char buf[] = "0x00000000";
  char *c = &buf[9];

  for (; n > 0; n = n >> 4) {
   *c-- = "0123456789ABCDEF"[n & 0xF];
  }

  s_puts(buf);
  return;
}

void printf(char *f, ...) {
  u8 check_next = 0;
  char *c;
  va_list ap;

  va_start(ap, f);
  for (; *f; f++) {
    if (check_next != 1 && *f == '%') {
      check_next = 1;
      continue;
    }

    if (check_next) {
      switch (*f) {
        case 'x':
          put_hex(va_arg(ap, u32));
          break;
        case 's':
          for (c = va_arg(ap, char *); *c; c++) s_putc(*c);
          break;
        default:
          s_putc(*f);
          break;
      }

      check_next = 0;
    } else {
      s_putc(*f);
    }
  }

  va_end(ap);
  return;
}
