#!/bin/bash

# find out where this is executing and cd to the project root
cd $(dirname $(realpath $0))
cd ..

# install packages
sudo pacman -Syy
sudo pacman -S nasm gcc binutils qemu-desktop ovmf git make mtools curl unzip

# install gnu-uefi headers
rm -rf gnu-efi
git clone https://www.github.com/somecollagist/gnu-efi-3.0.15
mv gnu-efi-3.0.15 gnu-efi
cd gnu-efi
make
cd ..

# OVMF provides a way of booting via UEFI for qemu, so copy it and make us the owner
sudo cp -r /usr/share/ovmf/x64 ./OVMF
sudo chown -R --reference=README.md ./OVMF
