/**
 * @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>
 *
 * Minimal set of crucial operations for the FAT filesystem. Implementatiton is based off of 
 * Microsoft's FAT32 File System Specification Version 1.03. 
 */

#pragma once
#include "util.h"

/**
 * FAT entry attributes.
 */
typedef enum {
  READ_ONLY = 0x01,
  HIDDEN    = 0x02,
  SYSTEM    = 0x04,
  VOLUME_ID = 0x08,
  DIRECTORY = 0x10,
  ARCHIVE   = 0x20,
  LONG_NAME = 0x0F,
} fat_attr_t;

/**
 * FAT types.
 */
typedef enum {
  FAT12,
  FAT16,
  FAT32
} fat_t;

/**
 * BIOS parameter block fields.
 */
typedef struct __attribute__((packed)) {
  u16 byts_per_sec;
  u8  secs_per_cls;
  u16 resv_scs_cnt;
  u8  numb_fts;
  u16 root_ent_cnt;
  u16 totl_scs_16;
  u8  media;
  u16 fat_sze_16;
  u16 scs_per_trk;
  u16 num_hds;
  u32 hidden_sec;
  u32 totl_scs_32;
  // start of FAT32 bpb (not valid for FAT12/16)
  u32 fat_sze_32;
  u16 ext_flgs;
  u16 fs_ver;
  u32 root_cls;
} bpb_t;

/**
 * Directory entry fields.
 */
typedef struct __attribute__((packed)) {
  u8  name[11];
  u8  attr;
  u8  not_for_use;
  u8  crt_time_tnth;
  u16 crt_time;
  u16 crt_date;
  u16 lst_acc_date;
  u16 fs_cls_hi;
  u16 wrt_time;
  u16 wrt_date;
  u16 fs_cls_lo;
  u32 file_size_bytes;
} dir_t;

/**
 * General information regarding the FAT filesystem.
 */
typedef struct file_sys_info {
  u32 cls_size;
  u32 fat_sze;
  u32 totl_scs;
  u32 sze_of_fats;
  u32 root_dir_scs;
  u32 fst_data_sec;
  u32 cnt_of_clsts;
  u32 eoc_err_mark;
  u32 resv_scs_cnt;
  u32 vbr_sec_off;
  u16 byts_per_sec;
  u8  secs_per_cls;
  fat_t type;
} fat_info_t;

#ifdef DEBUG
/**
 * Prints values stored in bpb.
 *
 * @param *bpb pointer to bpb_t
 */
void dump_bpb(bpb_t *bpb);

/**
 * Prints values stored in fi.
 *
 * @param *fi pointer to fat_info_t
 */
void dump_fi(fat_info_t *fi);
#endif // DEBUG

/**
 * Evaluates bios parameter block to fill in values of the passed fat_into_t struct.
 *
 * @param *bpb pointer to bpb_t
 * @param *fi pointer to fat_info_t
 */
void determine_fat_info(bpb_t *bpb, fat_info_t *fi);

/**
 * Given the number of a cluster n, return the value stored in the next cluster in the FAT chain.
 * Returns FAT eoc/err mark if there is no next cluster.
 *
 * @param *fi pointer to fat_info_t
 * @param n current cluster number
 * @param buf address of sector read buffer
 * @return number of next cluster in chain
 */
u16 find_next_cls(fat_info_t *fi, u16 n, u32 buf);

/**
 * Given the number of a cluster n, return the absolute sector index of the cluster.
 *
 * @param *fi pointer to fat_info_t
 * @param n cluster number
 * @return absolute sector index of cluster
 */
u32 first_sec_of_cls(fat_info_t *fi, u16 n);
