CC=i686-elf-gcc
LD=i686-elf-ld
AS=i686-elf-as
GRUBMK=i686-elf-grub-mkrescue

CCFLAGS := -ffreestanding
LDFLAGS := -T link.ld

SRCS := $(wildcard *.c)
ASSRCS := boot.s 
OBJS := $(patsubst %.c, %.o, $(SRCS)) boot.o

TARGET=kernel.bin
ISO=mykern.iso 

all: $(ISO)

$(ISO): $(TARGET)
	cp $(TARGET) iso/boot/$(TARGET)
	cp grub/grub.cfg iso/boot/grub/grub.cfg

	$(GRUBMK) -o $@ iso

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.s 
	$(AS) $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(ISO)

test:
	qemu-system-i386 -cdrom $(ISO)

.PHONY: clean all test