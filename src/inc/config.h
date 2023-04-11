/**
 * @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>
 *
 * General configuration file for dAmarA.
 */

#pragma once

#define SEC_BUF_OFF 0x1000  // Sector buffer offset
#define RM_L_ADDR 0x90000   // Real mode kernel load address
#define PM_L_ADDR 0x100000  // Protected mode kernel load address
#define RD_L_ADDR 0x200000  // Ram disk load address

#define BOOT_DIR (const u8 *) "BOOT"
#define KERNEL_F (const u8 *) "BZIMAGE"
#define CMD_LINE (const u8 *) "root=/dev/sda2 quiet rw"
