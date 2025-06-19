SRCS := src/KShell/shell.c src/err/panic.c \
		src/err/tf.c src/idt/idt.c \
		src/idt/handlers/irq0.c src/idt/masker.c \
		src/io/asm.c src/io/keyin.c \
		src/io/printer.c src/main.c \
		src/mem/memset.c src/qemu/shutdown.c \
		src/types/nums.c src/utils/img.c \
		src/utils/katoi.c src/utils/ksleep.c \
		src/utils/kstrcmp.c src/wex/chkst.c \
		src/wex/exec.c src/wex/rdfv.c \
		src/wex/stwex.c src/wex/testexec.c \
		src/idt/gdt.c src/fileio/ATA.c \
		src/fileio/MBR.c src/idt/handlers/atairq.c \
		src/fileio/irqflags.c src/fileio/fat16_mnt.c \
		src/fileio/read/ls.c src/fileio/read/printconts.c \
		src/fileio/read/mkfile.c src/KShell/kcmp.c \
		src/mem/memcpy.c src/mem/memcmp.c \
		src/fileio/read/rmfile.c

NASMSRCS := src/boot.asm \
			src/idt/handlers/gdtf.asm \
			src/idt/handlers/goodasm.asm \
			src/idt/handlers/irq0a.asm \
			src/idt/handlers/irq1a.asm \
			src/idt/picr.asm \
			src/utils/sdn.asm \
			src/utils/shutdown.asm \
			src/idt/handlers/irq14.asm \
			src/idt/handlers/irq15.asm 

OBJS := $(patsubst %.asm, objs/%.o, $(NASMSRCS)) \
		$(patsubst %.c, objs/%.o, $(SRCS))

DEPFILES := $(patsubst %.o, %.o.d, $(OBJS))

TARGET=kernel.bin
ELF = kernel.elf
ISO=mykern.iso 

CLNTARGS := $(TARGET) $(OBJS) $(ISO) iso/boot/$(TARGET) $(ELF)