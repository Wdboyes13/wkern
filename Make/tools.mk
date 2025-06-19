ARCH=X86
MACH=VMQEMU


ifeq ($(ARCH),X86)

CC			=i686-elf-gcc
LD			=i686-elf-ld
GRUBMK		=i686-elf-grub-mkrescue
QEMU		=qemu-system-i386
OBJCOPY		=i686-elf-objcopy
NASM		=nasm

endif
