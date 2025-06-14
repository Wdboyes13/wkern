ARCH=X86

ifeq ($(ARCH),X86)
CC=i686-elf-gcc
LD=i686-elf-ld
AS=i686-elf-as
STRIP=i686-elf-strip
GRUBMK=i686-elf-grub-mkrescue
QEMU=qemu-system-i386
endif

CCFLAGS := -ffreestanding -I. -DARCH_$(ARCH) -O2 -fno-pie -nostdlib
LDFLAGS := -T link.ld

GITREM := origin main
MSG := -m "Updated $(shell date)"

ASSRCS := $(wildcard *.s) \
		  $(wildcard *.S)

SRCS := $(wildcard *.c) \
		$(wildcard types/*.c) \
		$(wildcard io/*.c) \
		$(wildcard utils/*.c) \
		$(wildcard err/*.c)

OBJS := $(patsubst %.c, %.o, $(SRCS)) \
		$(patsubst %.s, %.o, $(ASSRCS))

HEADS := $(wildcard *.h) \
		 $(wildcard types/*.h) \
		 $(wildcard io/*.h) \
		 $(wildcard utils/*.h) \
		 $(wildcard err/*.h)

SILENCE := > /dev/null 2>&1

TARGET=kernel.bin
ISO=mykern.iso 

all: fmt $(ISO)

$(ISO): $(TARGET)
	cp $(TARGET) iso/boot/$(TARGET)
	cp grub/grub.cfg iso/boot/grub/grub.cfg

	$(GRUBMK) -o $@ iso $(SILENCE)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
	$(STRIP) $@

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.s 
	$(AS) $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(ISO) iso/boot/$(TARGET)

test:
	$(QEMU) -cdrom $(ISO)

git:
	git add .
	git commit $(MSG)
	git push $(GITREM)

fmt:
	clang-format -i $(HEADS) $(SRCS)

.PHONY: clean all test