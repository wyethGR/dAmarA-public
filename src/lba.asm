; @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>

BITS 16

global _lba_read

section .text
; Func: _lba_read
;       Read sectors from drive 0x80 to given offset
;
; Input: sector number, count of sectors to read
; Clobbers: AX, DX, SI
;
_lba_read:
  push bx

  ; setup DAP options
  xor edx, edx
  mov edx, [esp+6]
  mov [DAP.sector], edx

  xor edx, edx
  mov edx, [esp+10]
  mov [DAP.count], dx

  xor edx, edx
  mov edx, [esp+14]
  mov [DAP.seg], dx

  xor edx, edx
  mov edx, [esp+18]
  mov [DAP.off], dx

  mov si, DAP 
  ; extended read
  mov dl, 0x80
  mov ah, 0x42
  int 0x13

  pop bx
  o32 ret

section .data
DAP:
  db 0x10    ; size of DAP
  db 0x00    ; unused, should be zero
.count:
  dw 0x1     ; number of sectors to read
.off:
  dw 0x0000  ; off: pointer to where sectors will be read to
.seg:
  dw 0x0000  ; seg: 
.sector:
  dd 0x0     ; sector to read low
  dd 0x0     ; sector to read high
