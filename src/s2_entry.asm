; @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>

BITS 16

extern _main

section .text.entry
start:
  xor ax, ax
  mov ds, ax
  mov es, ax 
  mov bx, 0x7E00

  ; stack is at 0x0000:0x7E00
  mov ss, ax
  mov sp, bx
  mov bp, sp

  cld

main:
  mov si, go
  call tty_write

  mov edx, DWORD [0x7DBE + 0x8]
  push edx
  call DWORD _main

  mov dx, 0x9000
  mov ss, dx
  mov ds, dx
  mov es, dx
  mov fs, dx
  mov gs, dx

  mov dx, 0xE000
  mov sp, dx

  jmp 0x9020:0x0000

hang:
  hlt
  jmp hang

go:
  db "GO!", 0x0A, 0x0D, 0x00

; Func: tty_write
;       Output a string to the console on page 0
;
; Input: SI = string 
; Clobbers: AX, BX, SI
;
tty_write:
  lodsb
.w:
  mov ah, 0xE
  mov bx, 0x0001
  int 0x10
.l:
  lodsb
  cmp al, 0x0
  jne .w
  ret
