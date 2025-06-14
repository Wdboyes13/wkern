include Make/files.mk 
include Make/flags.mk
include Make/tools.mk

SILENCE := 1>/dev/null
SILENCEALL := >/dev/null 2>&1

TARGET=kernel.bin
ISO=mykern.iso 

CLNTARGS := $(TARGET) $(OBJS) $(ISO) iso/boot/$(TARGET)

all: fmt $(ISO)

$(ISO): $(TARGET)
	@echo "[CP]"
	@cp $(TARGET) iso/boot/$(TARGET)
	@echo "[CP]"
	@cp grub/grub.cfg iso/boot/grub/grub.cfg

	@echo "[GRUBMK] $@"
	@$(GRUBMK) -o $@ iso $(SILENCEALL)

$(TARGET): $(OBJS)
	@echo "[LD] $@"
	@$(LD) $(LDFLAGS) -o $@ $^
	# @echo "[STRIP] $@"
	# @$(STRIP) $@

%.o: %.c
	@echo "[CC] $<"
	@$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.s 
	@echo "[AS] $<"
	@$(AS) $< -o $@

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
