#!/bin/bash

# @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com>

git clone "https://github.com/torvalds/linux.git"
cd linux
git checkout v6.1
cp ../linux-6.1.config .config

make -j$(nproc)
mv arch/x86/boot/bzImage ../bzImage
