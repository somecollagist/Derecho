export OS		:= Derecho
IMG				:= $(OS).img

export ROOT		:= $(CURDIR)
export SRC		:= $(ROOT)/src
export BOOTSRC	:= $(ROOT)/boot
export GNUEFI	:= $(ROOT)/gnu-efi
export BIN		:= $(ROOT)/bin
export BOOTBIN	:= $(ROOT)/bootbin

export CC		:= gcc
export AS		:= nasm
export LD		:= ld

export CWARNS	:= -Wall -Wno-implicit-function-declaration -Wno-comment

LODEV			:= /dev/loop30
LOEFI			:= /dev/loop30p1
LOEFI_MNT		:= /mnt/$(OS)_efi

# .SILENT: prebuild build clean

all: prebuild build

prebuild:
	clear
	rm -rf $(BIN) $(BOOTBIN) $(IMG)
	mkdir $(BIN) $(BOOTBIN)

build:
	make -C $(BOOTSRC)
	make -C $(SRC)

	dd if=/dev/zero of=$(IMG) bs=1M count=256
	sudo gdisk $(IMG) < $(ROOT)/gdiskcmds
	sudo losetup -P $(LODEV) $(IMG)
	
	sudo mformat -i $(LOEFI) -f 1440 ::

	sudo mount --mkdir $(LOEFI) $(LOEFI_MNT)

	sudo mkdir $(LOEFI_MNT)/EFI
	sudo mkdir $(LOEFI_MNT)/EFI/BOOT
	sudo cp $(BOOTBIN)/BOOTX64.EFI $(LOEFI_MNT)/EFI/BOOT/BOOTX64.EFI
	sudo cp startup.nsh $(LOEFI_MNT)/startup.nsh
	sudo cp $(BIN)/kernel.elf $(LOEFI_MNT)/kernel.elf
	
	sudo umount $(LOEFI_MNT)

	sudo fdisk -l $(LODEV)
	sudo losetup -d $(LODEV)

run:
	qemu-system-x86_64 \
		-L OVMF/ \
		-pflash OVMF/OVMF.fd \
		-net none \
		-usb $(IMG) \
		-vga std \
		-m 256M
		