ASSRCS := $(wildcard *.s) \
		  $(wildcard *.S)

SRCS := $(wildcard *.c) \
		$(wildcard types/*.c) \
		$(wildcard io/*.c) \
		$(wildcard utils/*.c) \
		$(wildcard err/*.c) \
		$(wildcard qemu/*.c) \
		$(wildcard wex/*.c)

OBJS := $(patsubst %.c, %.o, $(SRCS)) \
		$(patsubst %.s, %.o, $(ASSRCS))

HEADS := $(wildcard *.h) \
		 $(wildcard types/*.h) \
		 $(wildcard io/*.h) \
		 $(wildcard utils/*.h) \
		 $(wildcard err/*.h) \
		 $(wildcard qemu/*.h) \
		 $(wildcard wex/*.h)