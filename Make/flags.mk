include Make/tools.mk
CCFLAGS := -ffreestanding -I. -DARCH_$(ARCH) -D$(MACH) -O2 -fno-pie -nostdlib
LDFLAGS := -T link.ld


GITREM := origin main
MSG := -m "Updated $(shell date)"