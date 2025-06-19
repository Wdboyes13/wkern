include Make/tools.mk
CCFLAGS 		:= -ffreestanding -I./src -DARCH_$(ARCH) -D$(MACH) -fno-pie -nostdlib \
					-m32 -O2 -fno-omit-frame-pointer -g -MMD -MP
					
LDFLAGS 		:= -m elf_i386 -T link.ld -n  --oformat elf32-i386
NASMFLAGS 		:= -f elf32 -g -F dwarf


GITREM 			:= origin main
MSG 			:= -m "Updated $(shell date)"

SILENCE 		:= 1>/dev/null
SILENCEALL 		:= >/dev/null 2>&1


QEMUFLAGS		:= -machine pc,accel=tcg  \
				-drive file=mykern.iso,format=raw,media=cdrom,if=ide,index=2 \
				-drive file=fat16.img,format=raw,if=ide,index=0 \
				-boot d -gdb tcp::1234 -monitor stdio \
				-qmp tcp:localhost:4444,server,nowait -no-reboot -no-shutdown


BLUE			=\033[0;34m
RED				=\033[0;31m
GREEN			=\033[0;32m
YELLOW			=\033[0;33m
MAGENTA			=\033[0;35m
CYAN			=\033[0;36m
CLEAR			=\033[0m