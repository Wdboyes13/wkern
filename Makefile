include Make/files.mk 
include Make/flags.mk
include Make/tools.mk

all: fmt comptests $(ISO) mbtest

$(ISO): $(TARGET)
	@echo "[CP]"
	@cp $(ELF) iso/boot/$(ELF)
	@echo "[CP]"
	@cp grub/grub.cfg iso/boot/grub/grub.cfg

	@echo "[GRUBMK] $@"
	@$(GRUBMK) -o $@ iso $(SILENCEALL)

$(ELF): $(OBJS)
	@echo "[LD] $@"
	@$(LD) $(LDFLAGS) -o $@ $^

$(TARGET): $(ELF)
	@echo "[OBJCOPY $@]"
	@$(OBJCOPY) $(OBJCFLAGS) $< $@

objs/%.o: %.c
	@echo "[CC] $<"
	@$(CC) $(CCFLAGS) -c $< -o $@ -MF $@.d

objs/%.o: %.asm
	@echo "[NASM] $<"
	@$(NASM) $(NASMFLAGS) $< -o $@

.PHONY: all

include Make/fakes.mk
