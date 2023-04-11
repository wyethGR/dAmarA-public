; @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>

BITS 16

global _s_start
global _s_putc
global _s_puts

PORT equ 0x3F8

section .text
; Func: _s_start
;       Setup COMM 1 serial port for write
;       
;       Buad rate:  9600       
;       Char len:   8 bits
;       Stop bits:  2 bits
;       Parity:     None
;
; Input: None
; Clobbers: AX, DX
;
_s_start:
  push bx

  ; disable interrupts
  mov al, 0x00
  lea dx, [PORT+1]
  out dx, al

  ; enable DLAB bit
  mov al, 0x80
  lea dx, [PORT+3]  ; line control reg
  out dx, al

  ; set divisor, buad rate of 9600
  mov ax, 0x000C
  mov dx, PORT
  out dx, ax

  ; set LCR, clear DLAB
  ; char length = 8 bits
  ; stop bits   = 2 bits
  ; parity      = none
  mov al, 0x07
  lea dx, [PORT+3]  ; line control reg
  out dx, al

  ; disable FIFO
  mov al, 0x00
  lea dx, [PORT+2]
  out dx, al

  ; disable ints, set loopback mode for test
  mov ax, 0x001E
  lea dx, [PORT+4]
  out dx, ax

  ; send a byte
  mov al, 0xEA
  mov dx, PORT
  out dx, al

  ; read a byte
  mov dx, PORT
  in ax, dx
  cmp al, 0xEA
  ;jne hang        ; hang if we didn't get the same byte back

  ; got the same back so enable OUT1
  mov al, 0x07
  lea dx, [PORT+4]
  out dx, al

  pop bx
  o32 ret

; Func: _s_putc
;       Output a char through COMM 1
;
; Input: [esp+6] (frist param when called from C) = char
; Clobbers: AX, DX
;
_s_putc:
  push bx
.test_ready:
  mov dx, PORT+5
  in ax, dx
  test ax, 0x10
  jne .test_ready
.w:
  mov al, [esp+6]
  mov dx, PORT
  out dx, al

  pop bx
  o32 ret

; Func: _s_puts
;       Output a string through COMM 1
;
; Input: [esp+6] (frist param when called from C) = char*
; Clobbers: AX, DX, SI
;
_s_puts:
  push bx
  mov si, [esp+6]
.test_ready:
  mov dx, PORT+5
  in ax, dx
  test ax, 0x10
  jne .test_ready
.l:
  lodsb
  cmp al, 0x0
  jne .w

  pop bx
  o32 ret
.w:
  mov dx, PORT
  out dx, al
  jmp .test_ready
