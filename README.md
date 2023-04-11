## dAmarA
> From the French *démarrer*, meaning "to start up"

A simple x86 boot loader for bzImage Linux kernels. The provided utilities can be used to build a disk image that can be booted with QEMU. The included kernel and Busybox configs can be used to build a system that features a simple shell booted with dAmarA.

----------

### Requirements
> NASM (tested with 2.15) \
> qemu-system-x86 (tested with 7.2)  \
> GNU make (tested with 4.4)  \
> GCC (tested with 12.2, target used x86_64-pc-linux-gnu)

----------

### Building & Usage
After cloning, follow the steps outlined in `res/README.md` to build the rootfs and kernel. Once those resources are built, make targets can be found by running `make help` from the root directory:
```
$ make help               print this message
$ make all                create damara.bin
$ make disk               make bootable disk.img
$ make run                make and run disk.img
$ make clean              clean tmp files and remove disk.img
$ make clean-tmp          clean tmp files only
$ make clean-disk         clean disk.img only
```
\
\
Support for serial port debugging can be enabled or disabled by commenting out the following line in `Makefile`:
```
# comment out to remove debugging output
DEBUG = debug
```
\
\
The disk build script `res/build-disk.sh` creates a formatted disk image that can be run using QEMU. The script is invoked using the following two makefile targets:
```
$ make disk
$ make run
```
\
\
The disk is formatted into the following partitions defined in `res/partition-table`:
| Secotrs | Type | Usage |
| ------- | ---- | ------ |
| 0 | N\A | MBR |
| 1 - 2047 | N\A | Resv. for stage 2 bootloader code |
| 2048 - 67583 | FAT | boot partition |
| 67584 - END | EXT | root partition |

\
\
Damara searches for and attempts to boot the file `KERNEL_F` in the directory `BOOT_DIR`:
```
src/inc/config.h

#define BOOT_DIR (const u8 *) "BOOT"
#define KERNEL_F (const u8 *) "BZIMAGE"
#define CMD_LINE (const u8 *) "root=/dev/sda2 quiet rw"
```

Note that these names must be formatted in the way names are stored internally by the FAT filesystem. Some examples come from the [FAT hardware white paper](https://www.cs.fsu.edu/~cop4610t/assignments/project3/spec/fatspec.pdf):
```
“foo.bar”       -> “FOO BAR”
“FOO.BAR”       -> “FOO BAR”
“Foo.Bar”       -> “FOO BAR”
“foo”           -> “FOO “
“foo.”          -> “FOO “
“PICKLE.A”      -> “PICKLE A “
“prettybg.big”  -> “PRETTYBGBIG”
“.big”          -> illegal, DIR_Name[0] cannot be 0x20
```

----------

### Future Goals/TODO
* Refine how files are read into memory (the clusters of a file are read into a buffer before being copied into their final destination)
* Add support for initrd/initramfs
* Create a `grub-install` type utility and carry out more 'real-world' situations

----------

#### A Handful of Resources Used
* [osdev serial ports](https://wiki.osdev.org/Serial_Ports) 
* [stage 2 with C](https://stackoverflow.com/questions/63552783/when-i-call-a-asm-function-in-c-the-parameters-dont-appear-on-the-stack) 
* [x86 real mode memory map](https://wiki.osdev.org/Memory_Map_(x86)) 
* [setting up hard disk imgs](https://wiki.syslinux.org/wiki/index.php?title=Hard_disk_images)
* [roll your own bootloader](https://wiki.osdev.org/Rolling_Your_Own_Bootloader) 
* [digging through openBSD mbr](https://prefetch.net/blog/2006/09/09/digging-through-the-mbr/)
* [grub disassembled](https://thestarman.pcministry.com/asm/mbr/GRUB.htm)
* [uaf bootblock lecture](https://www.cs.uaf.edu/2011/fall/cs301/lecture/11_18_bootblock.html) 
* [interrupt list](https://www.ctyme.com/intr/int.htm) 
* [better interrupt list](https://web.archive.org/web/20210830163916/http://www.techhelpmanual.com/2-main_menu.html) 
* [osdev x86 cpu registers](https://wiki.osdev.org/CPU_Registers_x86) 
* [copy of grub legacy source](https://github.com/jezze/grub-legacy/tree/master) 
* [x86 lods instruction](https://www.aldeid.com/wiki/X86-assembly/Instructions/lodsb)
* [general tips for bootloader developement](https://stackoverflow.com/questions/32701854/%20boot-loader-doesnt-jump-to-kernel-code/32705076#32705076)
* [x86 boot linux kernel boot protocol](https://www.kernel.org/doc/html/latest/x86/boot.html)
* [real mode address space](https://wiki.osdev.org/Memory_Map_(x86))
* [FAT hardware white paper](https://www.cs.fsu.edu/~cop4610t/assignments/project3/spec/fatspec.pdf)

----------

#### Licence

The GPLv2 License (GPLv2)

Copyright (c) 2023 Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
