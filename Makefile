ARCH=X86
MACH=VMQEMU
ifeq ($(ARCH),X86)
CC=i686-elf-gcc
LD=i686-elf-ld
AS=i686-elf-as
STRIP=i686-elf-strip
GRUBMK=i686-elf-grub-mkrescue
QEMU=qemu-system-i386
endif

CCFLAGS := -ffreestanding -I. -DARCH_$(ARCH) -D$(MACH) -O2 -fno-pie -nostdlib
LDFLAGS := -T link.ld


GITREM := origin main
MSG := -m "Updated $(shell date)"

ASSRCS := $(wildcard *.s) \
		  $(wildcard *.S)

SRCS := $(wildcard *.c) \
		$(wildcard types/*.c) \
		$(wildcard io/*.c) \
		$(wildcard utils/*.c) \
		$(wildcard err/*.c) \
		$(wildcard qemu/*.c) \
		$(wildcard wex/*.c)

OBJS := $(patsubst %.c, %.o, $(SRCS)) \
		$(patsubst %.s, %.o, $(ASSRCS))

HEADS := $(wildcard *.h) \
		 $(wildcard types/*.h) \
		 $(wildcard io/*.h) \
		 $(wildcard utils/*.h) \
		 $(wildcard err/*.h) \
		 $(wildcard qemu/*.h) \
		 $(wildcard wex/*.h)

SILENCE := > /dev/null 2>&1

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
	@$(GRUBMK) -o $@ iso $(SILENCE)

$(TARGET): $(OBJS)
	@echo "[LD] $@"
	@$(LD) $(LDFLAGS) -o $@ $^
	@echo "[STRIP] $@"
	@$(STRIP) $@

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
	git commit $(MSG)
	git push $(GITREM)

fmt:
	@echo "[FMT]"
	@clang-format -i $(HEADS) $(SRCS)

.PHONY: clean all test
