include Make/tools.mk
CCFLAGS := -ffreestanding -I./src -DARCH_$(ARCH) -D$(MACH) -fno-pie -nostdlib -m32 -O2 -fno-omit-frame-pointer -g -MMD -MP
LDFLAGS := -m elf_i386 -T link.ld -n  --oformat elf32-i386
NASMFLAGS := -f elf32 -g -F dwarf
OBJCFLAGS := -O binary -j .multiboot -j .text \
		     -j .rodata -j .data \


GITREM := origin main
MSG := -m "Updated $(shell date)"

SILENCE := 1>/dev/null
SILENCEALL := >/dev/null 2>&1
