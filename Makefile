include Make/files.mk 
include Make/flags.mk
include Make/tools.mk

SILENCE := 1>/dev/null
SILENCEALL := >/dev/null 2>&1

TARGET=kernel.bin
ELF = kernel.elf
ISO=mykern.iso 

CLNTARGS := $(TARGET) $(OBJS) $(ISO) iso/boot/$(TARGET) $(ELF)

all: fmt comptests $(ISO)

mbtest:
	./multiboot-check.sh

$(ISO): $(TARGET)
	@echo "[CP]"
	@cp $(ELF) iso/boot/$(ELF)
	@echo "[CP]"
	@cp grub/grub.cfg iso/boot/grub/grub.cfg

	@echo "[GRUBMK] $@"
	@$(GRUBMK) -o $@ iso

kernel.elf: $(OBJS)
	@echo "[LD] $@"
	@echo "[OBJS TO LINK] $^"
	@$(LD) $(LDFLAGS) -o $@ $^

kernel.bin: kernel.elf
	@echo "[OBJCOPY $@]"
	@$(OBJCOPY) -O binary \
		-j .multiboot \
		-j .text \
		-j .rodata \
		-j .data \
		$< $@

objs/%.o: %.c
	@echo "[CC] $<"
	@$(CC) $(CCFLAGS) -c $< -o $@

objs/%.o: %.asm
	@echo "[NASM] $<"
	@$(NASM) -f elf32 -g -F dwarf $< -o $@

clean:
	@echo "[RM] $(CLNTARGS)"
	@rm -f $(CLNTARGS)

test:
	@echo "[QEMU]"
	@qemu-system-i386 \
		-machine pc,accel=tcg \
		-drive file=mykern.iso,format=raw,media=cdrom,if=ide,index=2 \
		-drive file=fat16.img,format=raw,if=ide,index=0 \
		-boot d \
		-gdb tcp::1234 \
		-monitor stdio \
		-qmp tcp:localhost:4444,server,nowait \
		-no-reboot -no-shutdown
git: clean
	git add .
	git commit $(MSG) $(SILENCE)
	git push $(GITREM) $(SILENCE)

fmt:
	@echo "[FMT]"
	@clang-format -i $(HEADS) $(SRCS)

comptests:
	cd tsts && ./tstall.sh $(NASM) $(CC)

.PHONY: clean all test git fmt comptests
