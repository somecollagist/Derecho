#!/bin/bash

# find out where this is executing and cd to the project root
cd $(dirname $(realpath $0))
cd ..

# install packages
sudo pacman -Syy
sudo pacman -S nasm gcc binutils qemu-desktop ovmf git make mtools curl unzip

# install gnu-uefi headers
rm -rf gnu-efi
curl https://sourceforge.net/code-snapshots/git/g/gn/gnu-efi/code.git/gnu-efi-code-8b018e67212957de176292f95718df48f49a418b.zip -o gnu-efi.zip
unzip gnu-efi.zip
rm gnu-efi.zip
mv gnu-efi-code-8b018e67212957de176292f95718df48f49a418b gnu-efi
cd gnu-efi
make
cd ..

# OVMF provides a way of booting via UEFI for qemu, so copy it and make us the owner
sudo cp -r /usr/share/ovmf/x64 ./OVMF
sudo chown -R --reference=README.md ./OVMF
