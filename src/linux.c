/**
 * @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>
 */

#include "inc/linux.h"

#ifdef DEBUG
#include "inc/debug.h"

void dump_lin_h(lin_h_t *lin_h) {
  printf("setup_sects;      %x\r\n", lin_h->setup_sects);
  printf("syssize;          %x\r\n", lin_h->syssize);
  printf("version;          %x\r\n", lin_h->version);
  printf("kernel_version;   %x\r\n", lin_h->kernel_version);
  printf("loadflags;        %x\r\n", lin_h->loadflags);
  printf("code32_start;     %x\r\n", lin_h->code32_start);
  printf("ramdisk_image;    %x\r\n", lin_h->ramdisk_image);
  printf("ramdisk_size;     %x\r\n", lin_h->ramdisk_size);
  printf("heap_end_ptr;     %x\r\n", lin_h->heap_end_ptr);
  printf("cmd_line_ptr;     %s\r\n", lin_h->cmd_line_ptr);
  printf("xloadflags;       %x\r\n", lin_h->xloadflags);
}
#endif
