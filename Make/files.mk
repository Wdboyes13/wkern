SRCS := KShell/kshcmp.c \
		KShell/shell.c \
		err/panic.c \
		err/tf.c \
		idt/idt.c \
		idt/irq0.c \
		idt/masker.c \
		io/asm.c \
		io/keyin.c \
		io/printer.c \
		ldints-c.c \
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
		idt/gdt.c

NASMSRCS := boot.asm \
			idt/handlers/gdtf.asm \
			idt/handlers/good_handler.asm \
			idt/handlers/goodasm.asm \
			idt/handlers/irq0a.asm \
			idt/handlers/irq1a.asm \
			idt/picr.asm \
			ldints.asm \
			utils/sdn.asm \
			utils/shutdown.asm

OBJS := $(patsubst %.c, %.o, $(SRCS)) \
		$(patsubst %.asm, %.o, $(NASMSRCS))
		