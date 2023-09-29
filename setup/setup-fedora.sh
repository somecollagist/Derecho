# credit goes to @tyhdefu for writing this, Johann is bad at git

#!/bin/bash

# find out where this is executing and cd to the project root
cd $(dirname $(realpath $0))
cd ..

# install packages
sudo dnf upgrade
sudo dnf install --refresh nasm gcc binutils qemu git make dosfstools curl unzip e2fsprogs gdisk
# (ovmf is a dependency of qemu)

# OVMF provides a way of booting via UEFI for qemu, so copy it
cp -r /usr/share/edk2/ovmf ./OVMF && \
mv ./OVMF/OVMF_CODE.fd ./OVMF/OVMF.fd
