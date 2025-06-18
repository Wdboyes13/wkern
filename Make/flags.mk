include Make/tools.mk
CCFLAGS := -ffreestanding -I. -DARCH_$(ARCH) -D$(MACH) -fno-pie -nostdlib -m32 -O0 -fno-omit-frame-pointer -g
LDFLAGS := -m elf_i386 -T link.ld -n  --oformat elf32-i386

GITREM := origin main
MSG := -m "Updated $(shell date)"