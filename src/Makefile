CFLG := -ffreestanding -fshort-wchar -fno-stack-protector
CSRC := $(shell find $(SRC) -name "*.c")
CTAR := $(patsubst $(SRC)/%,$(BIN)/%,$(patsubst %.c,%.o,$(CSRC)))

AFLG := 
ASRC := $(shell find $(SRC) -name "*.asm")
ATAR :=  $(patsubst $(SRC)/%,$(BIN)/%,$(patsubst %.asm,%.o,$(ASRC)))

LDFLG := -T linker.ld \
		-static -Bsymbolic -nostdlib

all: $(ATAR) $(CTAR)
	$(LD) $(LDFLG) $^ -o $(BIN)/kernel.elf

$(BIN)/%.o: $(SRC)/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLG) -c $< -o $@

$(BIN)/%.o: $(SRC)/%.asm
	mkdir -p $(shell dirname $@)
	$(AS) $(AFLG) $< -o $@