# @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>

CC = gcc
CFLAGS = -Wall -fleading-underscore -fno-exceptions -fno-asynchronous-unwind-tables -fno-builtin -fno-builtin-function -ffreestanding -nostartfiles -nolibc -nostdlib -fno-stack-protector -fno-pic -m16 -c

# comment out to remove debugging output
#DEBUG = debug

O = tmp
R = res
S = src

S2_OBJS = $O/lba.a.o $O/fat.o $O/util.o $O/linux.o $O/s2_main.o

ifdef DEBUG
CFLAGS += -DDEBUG
S2_OBJS += $O/serial.a.o $O/debug.o
endif

all: $O $O/damara.bin

run: $O disk
	qemu-system-x86_64 -drive format=raw,file=$R/disk.img

disk: $O $O/disk.img

$O/disk.img: $O/damara.bin
	sudo ./$R/build-disk.sh

$O/damara.bin: src/s1.asm $O/s2.bin
	nasm -I$O -f bin -o $O/damara.bin $<

$O/s2.bin: $O/s2_entry.a.o $O/s2_main.elf
	ld -melf_i386 -nostdlib -T $R/s2_bin.ld
	objcopy -O binary $O/s2.elf $O/s2.bin

$O/s2_main.elf: $(S2_OBJS)
	ld -melf_i386 -nostdlib -r -T $R/s2_main.ld $(S2_OBJS)

$O/%.o:
	$(CC) $(CFLAGS) $S/$*.c -o $@

$O/%.a.o:
	nasm -f elf32 $S/$*.asm -o $@

$O:
	mkdir -p tmp

clean: clean-tmp clean-disk

clean-tmp:
	rm -rf $O

clean-disk:
	rm -f $R/disk.img

help:
	@echo "make help		print this message"
	@echo "make all 		create damara.bin"
	@echo "make disk 		make bootable disk.img"
	@echo "make run 		make and run disk.img"
	@echo "make clean 		clean tmp files and remove disk.img"
	@echo "make clean-tmp 		clean tmp files only"
	@echo "make clean-disk		clean disk.img only"
