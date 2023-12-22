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
LOPRT			:= $(LODEV)p1
LOPRT_MNT		:= /mnt/$(OS)

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
	
	sudo mkfs.fat -F 32 $(LOPRT)

	sudo mount --mkdir $(LOPRT) $(LOPRT_MNT)

	sudo mkdir $(LOPRT_MNT)/EFI
	sudo mkdir $(LOPRT_MNT)/EFI/BOOT
	sudo cp $(BOOTBIN)/BOOTX64.EFI $(LOPRT_MNT)/EFI/BOOT/BOOTX64.EFI
	sudo cp startup.nsh $(LOPRT_MNT)/startup.nsh
	sudo cp $(BIN)/kernel.elf $(LOPRT_MNT)/kernel.elf
	
	sudo umount $(LOPRT_MNT)
	sudo rm -rf $(LOPRT_MNT)

	sudo gdisk -l $(LODEV)
	sudo losetup -d $(LODEV)

run:
	qemu-system-x86_64 \
		-L OVMF/ \
		-pflash OVMF/OVMF.fd \
		-net none \
		-usb $(IMG) \
		-vga std \
		-m 256M

debugrun:
	qemu-system-x86_64 \
		-L OVMF/ \
		-pflash OVMF/OVMF.fd \
		-net none \
		-usb $(IMG) \
		-vga std \
		-m 256M \
		-s -S