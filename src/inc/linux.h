/**
 * @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>
 *
 * Struct representing the Linux kernel header.
 */

#pragma once
#include "util.h"

/**
 * https://github.com/torvalds/linux/blob/master/arch/x86/include/uapi/asm/bootparam.h
 *
 * Adapted from Linux source tree. Provided under the following license:
 * SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
 */
typedef struct __attribute__((packed)) {
  u8  setup_sects;
  u16 root_flags;
  u32 syssize;
  u16 ram_size;
  u16 vid_mode;
  u16 root_dev;
  u16 boot_flag;
  u16 jump;
  u32 header;
  u16 version;
  u32 realmode_swtch;
  u16 start_sys_seg;
  u16 kernel_version;
  u8  type_of_loader;
  u8  loadflags;
  u16 setup_move_size;
  u32 code32_start;
  u32 ramdisk_image;
  u32 ramdisk_size;
  u32 bootsect_kludge;
  u16 heap_end_ptr;
  u8  ext_loader_ver;
  u8  ext_loader_type;
  u32 cmd_line_ptr;
  u32 initrd_addr_max;
  u32 kernel_alignment;
  u8  relocatable_kernel;
  u8  min_alignment;
  u16 xloadflags;
  u32 cmdline_size;
  u32 hardware_subarch;
  u64 hardware_subarch_data;
  u32 payload_offset;
  u32 payload_length;
  u64 setup_data;
  u64 pref_address;
  u32 init_size;
  u32 handover_offset;
  u32 kernel_info_offset;
} lin_h_t;

#ifdef DEBUG
/**
 * Prints the information stored in passed lin_h.
 *
 * @param *lin_h pointer to lin_h_t
 */
void dump_lin_h(lin_h_t *lin_h);
#endif // DEBUG
