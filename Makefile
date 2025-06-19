include Make/files.mk 
include Make/flags.mk
include Make/tools.mk

SILENCE := 1>/dev/null
SILENCEALL := >/dev/null 2>&1

all: fmt comptests $(ISO)

$(ISO): $(TARGET)
	@echo "[CP]"
	@cp $(ELF) iso/boot/$(ELF)
	@echo "[CP]"
	@cp grub/grub.cfg iso/boot/grub/grub.cfg

	@echo "[GRUBMK] $@"
	@$(GRUBMK) -o $@ iso $(SILENCEALL)

$(ELF): $(OBJS)
	@echo "[LD] $@"
#	@echo "[OBJS TO LINK] $^"
	@$(LD) $(LDFLAGS) -o $@ $^

$(TARGET): $(ELF)
	@echo "[OBJCOPY $@]"
	@$(OBJCOPY) -O binary \
		-j .multiboot \
		-j .text \
		-j .rodata \
		-j .data \
		$< $@

objs/%.o: %.c
	@echo "[CC] $<"
	@$(CC) $(CCFLAGS) -c $< -o $@ -MF $@.d

objs/%.o: %.asm
	@echo "[NASM] $<"
	@$(NASM) -f elf32 -g -F dwarf $< -o $@

.PHONY: all

include Make/fakes.mk
