CFLG := -I$(GNUEFI)/inc -I$(GNUEFI)/lib -I$(ROOT) \
		-lgnuefi -lefi \
		-ffreestanding -fshort-wchar -fno-stack-protector \
		$(CWARNS)
CSRC := $(shell find $(SRC) -name "*.c")
CTAR := $(patsubst $(SRC)/%,$(BIN)/%,$(patsubst %.c,%.o,$(CSRC)))

AFLG := 
ASRC := $(shell find $(SRC) -name "*.asm")
ATAR :=  $(patsubst $(SRC)/%,$(BIN)/%,$(patsubst %.asm,%.o,$(ASRC)))

LDFLG := -T linker.ld \
		-static -Bsymbolic -nostdlib

INC := $(addprefix -I ,$(shell dirname $(shell find $(SRC) -name "*.h") | tr ' ' '\n' | sort -u | xargs))

all: $(ATAR) $(CTAR)
	$(LD) $(LDFLG) $^ -o $(BIN)/kernel.elf
	objdump -D -M x86-64,intel -z $(BIN)/kernel.elf >> $(BIN)/kernel.txt

$(BIN)/%.o: $(SRC)/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLG) -c $< -o $@ $(INC)

$(BIN)/%.o: $(SRC)/%.asm
	mkdir -p $(shell dirname $@)
	$(AS) $(AFLG) $< -o $@