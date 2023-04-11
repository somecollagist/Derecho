CFLG := -I$(GNUEFI)/inc -I$(GNUEFI)/lib -I$(ROOT) -I$(SRC) \
		-lgnuefi -lefi \
		-fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar \
		-mno-red-zone -maccumulate-outgoing-args \
		$(CWARNS)
CSRC := $(shell find $(BOOTSRC) -name "*.c")
CTAR := $(patsubst $(BOOTSRC)/%,$(BOOTBIN)/%,$(patsubst %.c,%.o,$(CSRC)))

AFLG := -f elf64
ASRC := $(shell find $(BOOTSRC) -name "*.asm")
ATAR := $(patsubst $(BOOTSRC)/%,$(BOOTBIN)/%,$(patsubst %.asm,%.o,$(ASRC)))

INC := $(addprefix -I ,$(shell dirname $(shell find $(BOOTSRC) -name "*.h") | tr ' ' '\n' | sort -u | xargs))

LDFLG := -T$(GNUEFI)/gnuefi/elf_x86_64_efi.lds \
		-L$(GNUEFI)/x86_64/lib -L$(GNUEFI)/x86_64/gnuefi \
		-shared -Bsymbolic -nostdlib \
		-lgnuefi -lefi

all: $(ATAR) $(CTAR)
	$(LD) $(LDFLG) $(GNUEFI)/x86_64/gnuefi/crt0-efi-x86_64.o $^ -o $(BOOTBIN)/boot.so -lgnuefi -lefi
	objcopy -j .text \
			-j .sdata \
			-j .data \
			-j .dynamic \
			-j .dynsym \
			-j .rel \
			-j .rela \
			-j .rel.* \
			-j .rela.* \
			-j .reloc \
			--target efi-app-x86_64 --subsystem=10 $(BOOTBIN)/boot.so $(BOOTBIN)/BOOTX64.EFI

$(BOOTBIN)/%.o: $(BOOTSRC)/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLG) -c $< -o $@ $(INC)

$(BOOTBIN)/%.o: $(BOOTSRC)/%.asm
	mkdir -p $(shell dirname $@)
	$(AS) $(AFLG) $< -o $@
