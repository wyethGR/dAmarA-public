# Contents
1. build-disk.sh - Used to build minimal Linux system image bootable by QEMU.
2. build-kernel.sh - Used to build minimal Linux kernel. Clones Linux source into current directory.
3. build-rootfs.sh - Used to build minimal rootfs with Busybox shell. Clones Busybox source into current directory.
4. linux-6.1.config - Kernel configuration file.
5. busybox-1.36.config - Busybox configuration file.
6. partition-table - Partition table describing the output image of `build-disk.sh`.
6. s2_bin.ld - Linker script used in building stage 2 binary.
7. s2_main.ld - Linker script used in building stage 2 C code.

### Building
It is assumed that all resources are being built on x86. If you are building on a different architecture, it is up to you to configure a cross compiler.
```
$ ./build-rootfs.sh
$ ./build-kernel.sh
```
After the rootfs and kernel are built, the rest of the of the system can be built using the `Makefile` in the root directory.

### Notes
`build-disk.sh` depends on the rootfs. Therefore, either run the included `build-rootfs.sh` from this directory or provide the contents of a rootfs.

`build-kernel.sh` clones the Linux kernel source tree. If you have a copy of the source and wish to avoid cloning the ~4GB repository, the included `linux-6.1.config` can be used to build the kernel. Simply move the built `bzImage` from `linux/arch/x86/boot/bzImage` to this directory.

`build-rootfs.sh` clones the Busybox source tree. In the same manner as the kernl, the included `busybox-1.36.config` can be used to build the Busybox shell. However, you will have to modify the config to point to the `rootfs` folder from where your Busybox source is located.
