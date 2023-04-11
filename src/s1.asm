; @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>

BITS 16
ORG 0x7C00

S2_SECS equ (s2_end - s2_start + 511) / 512
S2_OFF equ 0x7E00

__start:
  cli
  xor ax, ax

  ; tiny memory model, cs=ds=ss=es=0
  mov ds, ax
  mov es, ax 
  mov bx, 0x7C00  ; stack is just below bootloader

  mov ss, ax
  mov sp, bx
  mov bp, sp

enable_a20:
  push dx
  push ax

  mov dx, 0x92
  in al, dx
  test al, 2
  jnz enabled
  or al, 2
  and al, 0xFE

  mov dx, 0x92
  out dx, al

enabled:
  pop ax
  pop dx

; from osdev wiki, unreal mode
unreal:
  push ds               ; save real mode
 
  lgdt [gdtinfo]        ; load gdt register
 
  mov eax, cr0          ; switch to pmode by
  or al,1               ; set pmode bit
  mov cr0, eax
 
  jmp $+2               ; tell 386/486 to not crash
 
  mov bx, 0x08          ; select descriptor 1
  mov ds, bx            ; 8h = 1000b
 
  and al,0xFE           ; back to realmode
  mov cr0, eax          ; by toggling bit again
 
  pop ds                ; get back old segment
  cld

main:
  mov ah, 0x41    ; test for int 13h extensions (lba)
  mov bx, 0x55AA
  int 0x13

  ; CF set on error (extensions not supported)
  jnc l
  mov si, drive_err_str
  call tty_write
l:
  cmp bx, 0xAA55  ; 0xAA55 if installed
  je l1
  mov si, drive_err_str
  call tty_write
l1:

  mov si, damara_welcome_str
  call tty_write

lba_read:
  ; read in stage 2
  mov si, DAP 

  ; extended read
  mov ah, 0x42
  int 0x13

  jnc load_stage2
  mov si, lba_read_err_str
  call tty_write

load_stage2:
  jmp 0x0000:S2_OFF

hang:
  hlt
  jmp hang

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

damara_welcome_str: 
  db "dAmarA: ready, ",  0x00
drive_err_str:
  db "INT 13h ext. not supported", 0x0A, 0x0D, 0x00
lba_read_err_str:
  db "LBA read error", 0x0A, 0x0D, 0x00

DAP:
  db 0x10    ; size of DAP
  db 0x00    ; unused, should be zero
  dw S2_SECS ; number of sectors to read
  dw S2_OFF  ; off: pointer to where sectors will be read to
  dw 0x0     ; seg: 
  dd 0x1     ; sector to read (low)
  dd 0x0     ; sector to read (high)

; from osdev wiki, unreal mode
gdtinfo:
  dw gdt_end - gdt - 1   ;last byte in table
  dd gdt                 ;start of table
 
gdt:
  dd 0,0        ; entry 0 is always unused
flatdesc:
  db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0
gdt_end:

pad:
  ; Pad data out until magic boot signature. Declare 0x00s from where we are 
  ; ($-$$) up to the boot signature in last two bytes
  times 512-2-($-$$) db 0x00
  db 0x55
  db 0xAA

s2_start:
  incbin "s2.bin"
s2_end:
