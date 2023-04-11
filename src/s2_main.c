/**
 * @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>
 */

#include "inc/config.h"
#include "inc/util.h"
#include "inc/fat.h"
#include "inc/linux.h"
#include <stddef.h>

#ifdef DEBUG
#include "inc/debug.h"
#endif /* DEBUG */

/**
 * Loads secs sectors from the file system starting at given cluster. Sectors are loaded starting 
 * at the address d_base. Returns 
 *
 * @param *fi pointer to fat_info_t
 * @param *d_base destination base address
 * @param *cluster starting cluster number
 * @param secs number of sectors to read
 * @return number of sectors that didn't fit evenly into last cluster
 */
u32 read_blob(fat_info_t *fi, u8 *d_base, u16 *cluster, u32 secs) {
  u32 clstrs_to_read, secs_rem, clstrs_read, i, sec;
  u8 *d, *s;

  clstrs_to_read = secs / fi->secs_per_cls;
  secs_rem       = secs % fi->secs_per_cls;
  clstrs_read    = 0;

  #ifdef DEBUG
  printf("cls_to_read: %x\r\n", clstrs_to_read);
  printf("secs_rem:    %x\r\n", secs_rem);
  #endif /* DEBUG */

  for (i = 0; i < clstrs_to_read; i++) {
    sec = first_sec_of_cls(fi, *cluster);
    read_sectors(sec+fi->vbr_sec_off, fi->secs_per_cls);

    d = (u8 *) d_base + ((clstrs_read++) * fi->cls_size);
    s = (u8 *) SEC_BUF_OFF;
    memcpy(d, s, fi->cls_size);

    *cluster = find_next_cls(fi, *cluster, SEC_BUF_OFF);
  }

  if (secs_rem > 0) {
    sec = first_sec_of_cls(fi, *cluster);
    read_sectors(sec+fi->vbr_sec_off, fi->secs_per_cls);

    d = (u8 *) d_base + (clstrs_read * fi->cls_size);
    s = (u8 *) SEC_BUF_OFF;
    memcpy(d, s, fi->byts_per_sec*secs_rem);
  }

  return secs_rem;
}

/**
 * Scans directory entries within passed directory for a file matching the given name. Returns the 
 * file's respective cluster if found.
 *
 * @param *fi pointer to fat_info_t
 * @param *f_name name of file formatted accordding to FAT whitepaper
 * @param n_len length in bytes of name
 * @param *dir pointer to FAT directory entry to search
 * @return cluster number of file
 */
u32 find_file(fat_info_t *fi, const u8 *f_name, size_t n_len, dir_t *dir) {
  u32 cluster, sector_of_cls;

  cluster = (dir->fs_cls_hi << 16) | dir->fs_cls_lo;
  while (cluster && cluster < fi->eoc_err_mark) {
    sector_of_cls = first_sec_of_cls(fi, cluster);
    read_sectors(sector_of_cls+fi->vbr_sec_off, fi->secs_per_cls);

    dir = (dir_t *) SEC_BUF_OFF;
    while (dir < (dir_t *) SEC_BUF_OFF + fi->cls_size) {
      if (strncmp(f_name, dir->name, n_len) == 0) {

        #ifdef DEBUG
        printf("%s entry found\r\n", (char *) dir->name);
        #endif /* DEBUG */

        return (dir->fs_cls_hi << 16) | dir->fs_cls_lo;
      }
 
      dir++;
    }

    cluster = find_next_cls(fi, cluster, SEC_BUF_OFF);
  }
  
  return 0;
}

void main(u32 vbr_sec_off) {
  u32 fst_root_dir_sec, secs_rem, sys_size;
  u16 cluster;
  u8 setup_secs;
  fat_info_t fi;
  lin_h_t lin_h;
  bpb_t *bpb;
  dir_t *dir;

  #ifdef DEBUG
  s_start();
  #endif /* DEBUG */

  fi.vbr_sec_off = vbr_sec_off;
  // read in VBR
  #ifdef DEBUG
  printf("vbr_sec_off     %x\r\n\n", vbr_sec_off);
  printf("Reading in VBR...\r\n");
  #endif /* DEBUG */

  read_sectors(vbr_sec_off, 1);

  // offset 11 bc bpb_t type doesn't account for first two fields in the actual bpb structure
  bpb = (bpb_t *) (SEC_BUF_OFF + 11);
  determine_fat_info(bpb, &fi);

  #ifdef DEBUG
  printf("\r\nBPB info...\r\n");
  dump_bpb(bpb);
  printf("\r\nFat info...\r\n");
  dump_fi(&fi);
  #endif /* DEBUG */

  // read in root dir sector
  if (fi.type == FAT32) fst_root_dir_sec = first_sec_of_cls(&fi, bpb->root_cls);
  else                  fst_root_dir_sec = fi.resv_scs_cnt + fi.sze_of_fats;
  read_sectors(fst_root_dir_sec+vbr_sec_off, 1);

  // attempt to find BOOT directory
  dir = (dir_t *) SEC_BUF_OFF;
  while (dir->name[0] && dir < (dir_t *) SEC_BUF_OFF + fi.byts_per_sec) {
    if (dir->attr == DIRECTORY && strncmp(BOOT_DIR, (const u8 *) dir->name, strlen(BOOT_DIR)-1) == 0) {
      break;
    } 

    dir++;
  }

  cluster = find_file(&fi, KERNEL_F, strlen(KERNEL_F), dir);
  read_sectors(first_sec_of_cls(&fi, cluster)+vbr_sec_off, 2);

  // copying the lin_h header back and forth like this is wasteful
  memcpy((u8 *) &lin_h, (u8 *) (SEC_BUF_OFF+0x1F1), sizeof(lin_h_t));
  lin_h.type_of_loader = 0xFF;
  lin_h.loadflags |= 0x80;
  lin_h.heap_end_ptr = 0xE000-0x200;
  sys_size = lin_h.syssize;
  lin_h.cmd_line_ptr = 0xA0000 - lin_h.cmdline_size;
  memcpy((u8 *) lin_h.cmd_line_ptr, CMD_LINE, lin_h.cmdline_size);

  #ifdef DEBUG
  printf("\r\nKernel header info...\r\n");
  dump_lin_h(&lin_h);
  #endif /* DEBUG */

  setup_secs = lin_h.setup_sects;
  if (setup_secs == 0) setup_secs = 4;
  setup_secs++;

  // read in setup/real mode code
  #ifdef DEBUG
  printf("\r\n\tloading kernel...\r\n");
  #endif /* DEBUG */
  secs_rem = read_blob(&fi, (u8 *) RM_L_ADDR, &cluster, setup_secs);
  memcpy((u8 *) RM_L_ADDR+0x1F1, (u8 *) &lin_h, sizeof(lin_h_t));

  // load 32-bit protected mode portion
  // starts at (setup_secs+1)*512 in kernel image
  u8 *d = (u8 *) PM_L_ADDR;
  u8 *s = (u8 *) SEC_BUF_OFF + (secs_rem*fi.byts_per_sec);
  u32 off = (fi.secs_per_cls - secs_rem) * fi.byts_per_sec;
  memcpy(d, s, off);

  cluster = find_next_cls(&fi, cluster, SEC_BUF_OFF);
  read_blob(&fi, (u8 *) PM_L_ADDR + off, &cluster, (sys_size*16) / fi.byts_per_sec);
  
  return;
}
