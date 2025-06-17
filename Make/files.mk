ASSRCS := $(wildcard *.s) \
		  $(wildcard idt/*.s)

SRCS := $(wildcard *.c) \
		$(wildcard types/*.c) \
		$(wildcard io/*.c) \
		$(wildcard utils/*.c) \
		$(wildcard err/*.c) \
		$(wildcard qemu/*.c) \
		$(wildcard wex/*.c) \
		$(wildcard mem/*.c) \
		$(wildcard idt/*.c) \
		$(wildcard KShell/*.c)

NASMSRCS := $(wildcard *.asm)

OBJS := $(patsubst %.s, %.o, $(ASSRCS)) \
		$(patsubst %.c, %.o, $(SRCS)) \
		$(patsubst %.asm, %.o, $(NASMSRCS))
		

HEADS := $(wildcard *.h) \
		 $(wildcard types/*.h) \
		 $(wildcard io/*.h) \
		 $(wildcard utils/*.h) \
		 $(wildcard err/*.h) \
		 $(wildcard qemu/*.h) \
		 $(wildcard wex/*.h) \
		 $(wildcard mem/*.h) \
		 $(wildcard idt/*.h) \
		 $(wildcard KShell/*.h)