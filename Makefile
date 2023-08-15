export OS		:= Derecho
IMG				:= $(OS).img

export ROOT		:= $(CURDIR)
export SRC		:= $(ROOT)/src
export BOOTSRC	:= $(ROOT)/boot
export GNUEFI	:= $(ROOT)/gnu-efi
export BIN		:= $(ROOT)/bin
export BOOTBIN	:= $(ROOT)/bootbin
export ISO		:= $(ROOT)/iso

export CC		:= gcc
export AS		:= nasm
export LD		:= ld

export CWARNS	:= -Wall -Wno-implicit-function-declaration -Wno-comment

# .SILENT: prebuild build clean

all: prebuild build

prebuild:
	clear
	rm -rf $(BIN) $(BOOTBIN) $(IMG)
	mkdir $(BIN) $(BOOTBIN)

build:
	make -C $(BOOTSRC)
	make -C $(SRC)

	dd if=/dev/zero of=$(IMG) bs=1M count=16
	sudo losetup -fP --show $(IMG) > /tmp/dlos.txt
	$(eval LODEV := $(shell cat /tmp/dlos.txt))
	sudo fdisk $(LODEV) < $(ROOT)/fdiskcmds
	sudo mformat -i $(LODEV)p1 -f 1440 ::
	# sudo mkfs.ext2 $(LODEV)p2
	
	# EFI
	sudo mount $(LODEV)p1 /mnt
	sudo mkdir /mnt/EFI
	sudo mkdir /mnt/EFI/BOOT
	sudo cp $(BOOTBIN)/BOOTX64.EFI /mnt/EFI/BOOT/BOOTX64.EFI
	sudo cp startup.nsh /mnt/startup.nsh
	sudo cp $(BIN)/kernel.elf /mnt/kernel.elf
	sudo umount /mnt

	# # ext2
	# sudo mount $(LODEV)p2 /mnt
	# sudo cp -r $(ISO) /mnt
	# sudo umount /mnt
	
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
		