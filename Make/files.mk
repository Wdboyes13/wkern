SRCS := KShell/shell.c \
		err/panic.c \
		err/tf.c \
		idt/idt.c \
		idt/handlers/irq0.c \
		idt/masker.c \
		io/asm.c \
		io/keyin.c \
		io/printer.c \
		main.c \
		mem/memset.c \
		qemu/shutdown.c \
		types/nums.c \
		utils/img.c \
		utils/katoi.c \
		utils/ksleep.c \
		utils/kstrcmp.c \
		wex/chkst.c \
		wex/exec.c \
		wex/rdfv.c \
		wex/stwex.c \
		wex/testexec.c \
		idt/gdt.c \
		fileio/ATA.c \
		fileio/MBR.c \
		idt/handlers/atairq.c \
		fileio/irqflags.c \
		fileio/fat16_mnt.c \
		fileio/read/ls.c \
		fileio/read/printconts.c

NASMSRCS := boot.asm \
			idt/handlers/gdtf.asm \
			idt/handlers/goodasm.asm \
			idt/handlers/irq0a.asm \
			idt/handlers/irq1a.asm \
			idt/picr.asm \
			utils/sdn.asm \
			utils/shutdown.asm \
			idt/handlers/irq14.asm \
			idt/handlers/irq15.asm 

OBJS := $(patsubst %.asm, objs/%.o, $(NASMSRCS)) \
		$(patsubst %.c, objs/%.o, $(SRCS))

DEPFILES := $(patsubst %.o, %.o.d, $(OBJS))