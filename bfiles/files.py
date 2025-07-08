##
# @file files.py
# @brief Defines file organization and module structure for the build system.
#
# This file contains lists of source files (C and ASM), grouped into modules.
# It also defines output targets, build metadata, and clean targets used by
# other parts of the build system.
#
# @details
# The build system uses this file to:
# - Track all C source files (`SRCS`)
# - Track all NASM source files (`NASMSRCS`)
# - Define how C source files are grouped into logical modules (`MODULES`)
# - Generate output ELF files and ISO builds
# - Determine which object files need to be cleaned (`CLEANTARGS`)
#
# This file is imported by other scripts such as `build.py`, `brules.py`,
# and `frules.py` to control compilation, linking, and packaging.
#
# @author Will
# @date 2025
# @version 1.0
#
# @note You must update this file whenever a new source file or module
#       is added to the project.

import glob
import build
import os

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

    "src/mem/alloc.c",
    "src/mem/memutil.c",
    "src/err/tf.c",
    "src/err/panic.c",
    "src/wex/testexec.c",
    "src/wex/wexent.c",
    "src/wex/wexexec.c",
    "src/utils/img.c",

    "src/io/keyin.c",
    "src/io/printer.c",
    "src/io/asm.c",
    "src/utils/strings.c",
    "src/utils/ksleep.c",
    "src/utils/numtools.c",
    "src/main.c",
    "src/qemu/shutdown.c",

    "src/idt/gdt.c",
    "src/idt/idt.c",
    "src/idt/pit.c",
    "src/idt/handlers/irq0.c",
    "src/idt/masker.c",
    "src/pci/scconfig.c",
    "src/net/virtio.c",
    "src/idt/handlers/virtnetirq.c",

    "src/KShell/shell.c",
    "src/KShell/shellhelp.c",
    "src/KShell/filecmds.c",
    "src/KShell/regexcmd.c",
    "src/slre/slre.c",
]

if build.ARCH == "X86":
    NASMSRCS = [
        "src/boot.asm",
        "src/idt/handlers/gdtf.asm",
        "src/idt/handlers/irq0a.asm",
        "src/idt/handlers/irq1a.asm",
        "src/idt/handlers/virtirq.asm",
        "src/idt/picr.asm"
    ]
elif build.ARCH == "X64":
    NASMSRCS = [
        "src/boot64.asm",
        "src/idt/handlers/gdtf64.asm",
        "src/idt/handlers/irq0a64.asm",
        "src/idt/handlers/irq1a64.asm",
        "src/idt/picr.asm"
    ]
else: 
    print("Unsupported Arch")
    os.exit(1)

# Most are grouped into 1 except for larger systems
MODULES = {
    "KShell": [     "src/KShell/shell.c",
                    "src/KShell/shellhelp.c",
                    "src/KShell/filecmds.c",
                    "src/KShell/regexcmd.c",
                    "src/slre/slre.c"],

    "FileIO": [     "src/fileio/irqflags.c",
                    "src/fileio/ops/printconts.c",
                    "src/fileio/ops/ls.c",
                    "src/fileio/ops/mkfile.c",
                    "src/fileio/ops/rmfile.c",
                    "src/fileio/ops/write.c",
                    "src/fileio/ATA.c",
                    "src/fileio/fat16_mnt.c",
                    "src/fileio/MBR.c"],
    
    "CPUSub": [     "src/idt/gdt.c",
                    "src/idt/idt.c",
                    "src/idt/pit.c",
                    "src/idt/handlers/irq0.c",
                    "src/idt/masker.c",
                    "src/pci/scconfig.c",
                    "src/net/virtio.c",
                    "src/idt/handlers/virtnetirq.c"],

    "Core": [       "src/io/keyin.c",
                    "src/io/printer.c",
                    "src/io/asm.c",
                    "src/utils/strings.c",
                    "src/utils/ksleep.c",
                    "src/utils/numtools.c",
                    "src/main.c",
                    "src/qemu/shutdown.c"],

    "Misc": [       "src/mem/alloc.c",
                    "src/mem/memutil.c",
                    "src/err/tf.c",
                    "src/err/panic.c",
                    "src/wex/testexec.c",
                    "src/wex/wexent.c",
                    "src/wex/wexexec.c",
                    "src/utils/img.c"]
}

OBJS = [f"objs/modules/{mod}.o" for mod in MODULES]

for src in NASMSRCS:
    OBJS.append("objs/" + src.replace(".asm", ".o"))

OUT = "kernel.elf"
OUTARG = "iso/boot/" + OUT
ISO = "mykern.iso"

if build.ARCH == "X86":
    GRUBCFG = "grub/grub.cfg"
    GRUBCFGTARG = "iso/boot/" + GRUBCFG
elif build.ARCH == "X64":
    GRUBCFG = "grub/64/grub.cfg"
    GRUBCFGTARG = "iso/boot/grub/grub.cfg"

HEADS = ["src/types/nums.h", "src/fileio/fileio.h",
			   "src/global.h", "src/err/kerror.h",
			   "src/wex/stdwex.h", "src/io/kio.h",
			   "src/utils/util.h", "src/idt/idtirq.h",
			   "src/qemu/shutdown.h", "src/mem/kmem.h",
			   "src/KShell/shell.h", "src/KShell/filecmds.h",
               "src/pci/pci.h"
               ]

CLEANTARGS = [*OBJS, ISO, OUTARG, OUT, GRUBCFGTARG, "didconf",  *glob.glob("objs/**/*.o", recursive=True) ]