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

# .SILENT: prebuild build clean

all: prebuild build

prebuild:
	clear
	rm -rf $(BIN) $(BOOTBIN) $(IMG)
	mkdir $(BIN) $(BOOTBIN)

build:
	make -C $(BOOTSRC)
	make -C $(SRC)
	dd if=/dev/zero of=$(IMG) bs=1k count=1440
	mformat -i $(IMG) -f 1440 ::
	mmd -i $(IMG) ::/EFI
	mmd -i $(IMG) ::/EFI/BOOT
	mcopy -i $(IMG) $(BOOTBIN)/BOOTX64.EFI ::/EFI/BOOT
	mcopy -i $(IMG) $(BIN)/kernel.elf ::
	mcopy -i $(IMG) startup.nsh ::

run:
	qemu-system-x86_64 \
		-L OVMF/ \
		-pflash OVMF/OVMF.fd \
		-net none \
		-usb $(IMG) \
		-vga std \
		-m 256M


