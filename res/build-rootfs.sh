#!/bin/bash

# @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>

mkdir -p rootfs/bin
mkdir -p rootfs/dev
mkdir -p rootfs/etc/init.d
echo -e "#!/bin/sh\nmount -t proc proc /proc\nmount -t sysfs sysfs /sys" > rootfs/etc/init.d/rcS
chmod +x rootfs/etc/init.d/rcS
touch rootfs/etc/fstab
mkdir -p rootfs/proc
mkdir -p rootfs/sbin
mkdir -p rootfs/sys
mkdir -p rootfs/usr

git clone "https://github.com/mirror/busybox.git"
cd busybox
git checkout 1_36_stable
cp ../busybox-1.36.config .config
make -j$(nproc)
make install
cd ..
