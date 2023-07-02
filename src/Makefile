CFLG := -I$(GNUEFI)/inc -I$(GNUEFI)/lib -I$(ROOT) \
		-lgnuefi -lefi \
		-ffreestanding -fshort-wchar -fno-stack-protector \
		$(CWARNS)
CSRC := $(shell find $(SRC) -name "*.c")
CTAR := $(patsubst $(SRC)/%,$(BIN)/%,$(patsubst %.c,%.o,$(CSRC)))

AFLG := -f elf64
ASRC := $(shell find $(SRC) -name "*.asm")
ATAR := $(patsubst $(SRC)/%,$(BIN)/%,$(patsubst %.asm,%.o,$(ASRC)))

TSRC := $(shell find $(SRC) -name "*.txt")
TTAR := $(patsubst $(SRC)/%,$(BIN)/%,$(patsubst %.txt,%.o,$(TSRC)))

LDFLG := -T linker.ld \
		-static -Bsymbolic -nostdlib

INC := $(addprefix -I ,$(shell dirname $(shell find $(SRC) -name "*.h") | tr ' ' '\n' | sort -u | xargs))

all: $(ATAR) $(CTAR) $(TTAR)
	$(LD) $(LDFLG) $^ -o $(BIN)/kernel.elf
	objdump -D -M x86-64,intel -z $(BIN)/kernel.elf >> $(BIN)/kernel.txt

$(BIN)/%.o: $(SRC)/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLG) -c $< -o $@ $(INC)

$(BIN)/%.o: $(SRC)/%.asm
	mkdir -p $(shell dirname $@)
	$(AS) $(AFLG) $< -o $@

$(BIN)/%.o: $(SRC)/%.txt
	mkdir -p $(shell dirname $@)
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@
	objcopy -S $@
	objcopy --add-symbol $(basename $(notdir $@))=.data:0,global,export,object $@