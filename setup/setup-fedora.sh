# credit goes to @tyhdefu for writing this, Johann is bad at git

#!/bin/bash

# find out where this is executing and cd to the project root
cd $(dirname $(realpath $0))
cd ..

# install packages
sudo dnf upgrade
sudo dnf install --refresh nasm gcc binutils qemu git make mtools curl unzip e2fsprogs
# (ovmf is a dependency of qemu)

# install gnu-uefi headers
rm -rf gnu-efi
git clone https://www.github.com/somecollagist/gnu-efi-3.0.15
mv gnu-efi-3.0.15 gnu-efi
cd gnu-efi
make
cd ..

# OVMF provides a way of booting via UEFI for qemu, so copy it
cp -r /usr/share/edk2/ovmf ./OVMF && \
mv ./OVMF/OVMF_CODE.fd ./OVMF/OVMF.fd