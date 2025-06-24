SRCS = [
    "src/fileio/irqflags.c",
    "src/fileio/ops/printconts.c",
    "src/fileio/ops/ls.c",
    "src/fileio/ops/mkfile.c",
    "src/fileio/ops/rmfile.c",
    "src/fileio/ops/write.c",
    "src/fileio/ATA.c",
    "src/fileio/fat16_mnt.c",
    "src/fileio/MBR.c",
    "src/mem/memutil.c",
    "src/err/tf.c",
    "src/err/panic.c",
    "src/wex/testexec.c",
    "src/wex/wexent.c",
    "src/wex/wexexec.c",
    "src/io/keyin.c",
    "src/io/printer.c",
    "src/io/asm.c",
    "src/utils/strings.c",
    "src/utils/ksleep.c",
    "src/utils/numtools.c",
    "src/utils/img.c",
    "src/idt/gdt.c",
    "src/idt/idt.c",
    "src/idt/pit.c",
    "src/idt/handlers/irq0.c",
    "src/idt/masker.c",
    "src/main.c",
    "src/qemu/shutdown.c",
    "src/KShell/shell.c",
    "src/KShell/shellhelp.c",
    "src/KShell/filecmds.c",
    "src/mem/alloc.c",
    "src/KShell/regexcmd.c",
    "src/slre/slre.c",
    "src/pci/scconfig.c"
]

NASMSRCS = [
    "src/boot.asm",
    "src/idt/handlers/gdtf.asm",
    "src/idt/handlers/irq0a.asm",
    "src/idt/handlers/irq1a.asm",
    "src/idt/picr.asm"
]

OBJS = []

for src in NASMSRCS:
    OBJS.append("objs/" + src.replace(".asm", ".o"))

for src in SRCS:
    OBJS.append("objs/" + src.replace(".c", ".o"))

OUT = "kernel.elf"
OUTARG = "iso/boot/" + OUT
ISO = "mykern.iso"

GRUBCFG = "grub/grub.cfg"
GRUBCFGTARG = "iso/boot/" + GRUBCFG

HEADS = ["src/types/nums.h", "src/fileio/fileio.h",
			   "src/global.h", "src/err/kerror.h",
			   "src/wex/stdwex.h", "src/io/kio.h",
			   "src/utils/util.h", "src/idt/idtirq.h",
			   "src/qemu/shutdown.h", "src/mem/kmem.h",
			   "src/KShell/shell.h", "src/KShell/filecmds.h",
               "src/pci/pci.h"
               ]

CLEANTARGS = [*OBJS, ISO, OUTARG, OUT, GRUBCFGTARG, "didconf"]