/**
 * @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>
 */

#include "inc/fat.h"

#ifdef DEBUG
#include "inc/debug.h"

void dump_bpb(bpb_t *bpb) {
  printf("byts_per_sec;     %x\r\n", bpb->byts_per_sec);
  printf("secs_per_cls;     %x\r\n", bpb->secs_per_cls);
  printf("resv_scs_cnt;     %x\r\n", bpb->resv_scs_cnt);
  printf("numb_fts;         %x\r\n", bpb->numb_fts);
  printf("root_ent_cnt;     %x\r\n", bpb->root_ent_cnt);
  printf("totl_scs_16;      %x\r\n", bpb->totl_scs_16);
  printf("media;            %x\r\n", bpb->media);
  printf("fat_sze_16;       %x\r\n", bpb->fat_sze_16);
  printf("scs_per_trk;      %x\r\n", bpb->scs_per_trk);
  printf("num_hds;          %x\r\n", bpb->num_hds);
  printf("hidden_sec;       %x\r\n", bpb->hidden_sec);
  printf("totl_scs_32;      %x\r\n", bpb->totl_scs_32);
  printf("start of FAT32 bpb (not valid for FAT12/16)\r\n");
  printf("fat_sze_32;       %x\r\n", bpb->fat_sze_32);
  printf("ext_flgs;         %x\r\n", bpb->ext_flgs);
  printf("fs_ver;           %x\r\n", bpb->fs_ver);
  printf("root_cls;         %x\r\n", bpb->root_cls);
}

void dump_fi(fat_info_t *fi) {
  printf("cls_size;         %x\r\n", fi->cls_size);
  printf("fat_sze;          %x\r\n", fi->fat_sze);
  printf("totl_scs;         %x\r\n", fi->totl_scs);
  printf("sze_of_fats;      %x\r\n", fi->sze_of_fats);
  printf("root_dir_scs;     %x\r\n", fi->root_dir_scs);
  printf("fst_data_sec;     %x\r\n", fi->fst_data_sec);
  printf("cnt_of_clsts;     %x\r\n", fi->cnt_of_clsts);
  printf("eoc_err_mark;     %x\r\n", fi->eoc_err_mark);
  printf("resv_scs_cnt;     %x\r\n", fi->resv_scs_cnt);
  printf("vbr_sec_off;      %x\r\n", fi->vbr_sec_off);
  printf("byts_per_sec;     %x\r\n", fi->byts_per_sec);
  printf("secs_per_cls;     %x\r\n", fi->secs_per_cls);
  printf("fat_t;            %x\r\n", fi->type);
}
#endif /* DEBUG */

void determine_fat_info(bpb_t *bpb, fat_info_t *fi) {
  u32 data_scs_cnt;

  fi->cls_size = bpb->secs_per_cls * bpb->byts_per_sec;
  fi->resv_scs_cnt = bpb->resv_scs_cnt;
  fi->byts_per_sec = bpb->byts_per_sec;
  fi->secs_per_cls = bpb->secs_per_cls;

  fi->fat_sze = bpb->fat_sze_16;
  fi->totl_scs = bpb->totl_scs_16;

  // if fat16 size is zero, use fat32 size
  if (fi->fat_sze == 0) fi->fat_sze = bpb->fat_sze_32;
  // if fat16 total sectors is zero, use fat32 total sectors
  if (fi->totl_scs == 0) fi->totl_scs = bpb->totl_scs_32;

  fi->sze_of_fats = bpb->numb_fts*fi->fat_sze;
  fi->root_dir_scs = ((bpb->root_ent_cnt * sizeof(dir_t)) + (bpb->byts_per_sec-1)) / bpb->byts_per_sec;
  fi->fst_data_sec = fi->resv_scs_cnt + fi->sze_of_fats + fi->root_dir_scs;

  data_scs_cnt = fi->totl_scs - fi->resv_scs_cnt + fi->sze_of_fats + fi->root_dir_scs;
  fi->cnt_of_clsts = data_scs_cnt / bpb->secs_per_cls;

  if      (fi->cnt_of_clsts < 0xFF5)  fi->type = FAT12;
  else if (fi->cnt_of_clsts < 0xFFF5) fi->type = FAT16;
  else                                fi->type = FAT32;

  if      (fi->type == FAT12) fi->eoc_err_mark = 0x00000FF7;
  else if (fi->type == FAT16) fi->eoc_err_mark = 0x0000FFF7;
  else                        fi->eoc_err_mark = 0x0FFFFFF7;
}

u16 find_next_cls(fat_info_t *fi, u16 n, u32 buf) {
  u16 fat_offset, fat_sec_num, fat_ent_off, cls_ent_val;

  // check to see if we're at the last sector
  if (n > fi->cnt_of_clsts + 1) return fi->eoc_err_mark;

  if      (fi->type == FAT12) fat_offset = n + (n / 2);
  else if (fi->type == FAT16) fat_offset = n * 2;
  else                        fat_offset = n * 4;

  fat_sec_num = fi->resv_scs_cnt + (fat_offset / fi->byts_per_sec);
  fat_ent_off = fat_offset % fi->byts_per_sec;

  // read in sectors X and X+1 if it's FAT12
  if (fi->type == FAT12) read_sectors(fat_sec_num+fi->vbr_sec_off, 2);
  else                   read_sectors(fat_sec_num+fi->vbr_sec_off, 1);

  if (fi->type == FAT12) {
    cls_ent_val = IN(u16, buf+fat_ent_off);
    // we only want low 12 bits if n is even
    // high 12 bits if n is odd
    if (n & 0x0001) cls_ent_val = cls_ent_val >> 4;
    else            cls_ent_val &= 0x0FFF;
  } 
  else if (fi->type == FAT16) cls_ent_val = IN(u16, buf+fat_ent_off);
  else                        cls_ent_val = IN(u32, buf+fat_ent_off) & 0x0FFFFFFF;

  return cls_ent_val;
}

u32 first_sec_of_cls(fat_info_t *fi, u16 n) {
  return ((n - 2) * fi->secs_per_cls) + fi->fst_data_sec; 
}
