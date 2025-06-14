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