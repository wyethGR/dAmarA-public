#!/bin/bash

# @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>
#
# Creates file 'res/disk.img', a bootable image containing the boot loader, boot and rootfs 
# partitions. This file can be run using QEMU.

# terminate if a command has non-zero return value
set -e

# drop privileges back to non-root user
depriv() {
  if [[ $SUDO_USER ]]; then
    sudo -u "$SUDO_USER" -- "$@"
  else
    "$@"
  fi
}

function cleanup() {
  echo -e "\nCleaning up";
  umount -v tmp/loopback;
  rm -v -r tmp/loopback;
  losetup -v -d $lodev;
}

# make sure to cleanup on exit
trap cleanup EXIT

echo -e "Creating empty disk image..."
depriv dd if=/dev/zero of=res/disk.img bs=512 count=409600

lodev=$(losetup -f)
echo -e "\nSetup loopback dev $lodev"
losetup -v $lodev res/disk.img

echo -e "\nCopying damara.bin to start of $lodev"
dd if=tmp/damara.bin of=$lodev

echo -e "\nConfiguring FAT partition table..."
sfdisk $lodev < res/partition-table

echo -e "\nRunning partprobe..."
partprobe -s $lodev

echo -e "\nMaking FAT filesystem..."
lodevp1="${lodev}p1"
mkfs.fat $lodevp1 
fatlabel $lodevp1 "sda1"

echo -e "\nMounting partition $lodevp1"
mkdir -p -v tmp/loopback
mount -v -t vfat $lodevp1 tmp/loopback

echo -e "\nCopying kernel..."
mkdir -p -v tmp/loopback/boot
cp -v res/bzImage tmp/loopback/boot/
umount -v tmp/loopback

echo -e "\nMaking EXT filesystem..."
lodevp2="${lodev}p2"
mkfs.ext4 $lodevp2 
e2label $lodevp2 "sda2"

echo -e "\nMounting partition $lodevp2"
mkdir -p -v tmp/loopback
mount -v -t ext4 $lodevp2 tmp/loopback

echo -e "\nCopying rootfs..."
cp -r res/rootfs/* tmp/loopback/

exit 0
