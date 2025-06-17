include Make/files.mk 
include Make/flags.mk
include Make/tools.mk

SILENCE := 1>/dev/null
SILENCEALL := >/dev/null 2>&1

TARGET=kernel.bin
ELF = kernel.elf
ISO=mykern.iso 

CLNTARGS := $(TARGET) $(OBJS) $(ISO) iso/boot/$(TARGET) $(ELF)

all: fmt $(ISO)

$(ISO): $(TARGET)
	@echo "[CP]"
	@cp $(ELF) iso/boot/$(ELF)
	@echo "[CP]"
	@cp grub/grub.cfg iso/boot/grub/grub.cfg

	@echo "[GRUBMK] $@"
	@$(GRUBMK) -o $@ iso

kernel.elf: $(OBJS)
	@echo "[LD] $@"
	@$(LD) $(LDFLAGS) -o $@ $^

kernel.bin: kernel.elf
	@echo "[OBJCOPY $@]"
	@$(OBJCOPY) -O binary \
		-j .multiboot \
		-j .text \
		-j .rodata \
		-j .data \
		$< $@

%.o: %.c
	@echo "[CC] $<"
	@$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.s 
	@echo "[AS] $<"
	@$(AS) $< -o $@

%.o: %.asm
	@echo "[NASM] $<"
	@nasm -f elf32 -g -F dwarf $< -o $@

clean:
	@echo "[RM] $(CLNTARGS)"
	@rm -f $(CLNTARGS)

test:
	@echo "[QEMU]"
	@$(QEMU) -cdrom $(ISO) -machine q35

git:
	git add .
	git commit $(MSG) $(SILENCE)
	git push $(GITREM) $(SILENCE)

fmt:
	@echo "[FMT]"
	@clang-format -i $(HEADS) $(SRCS)

.PHONY: clean all test git fmt
