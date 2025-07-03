##
# @file frules.py
# @brief Build system utility rules for formatting, Git, run, and header validation.
#
# This file defines helper routines used during the build pipeline:
# - Code formatting with `clang-format`
# - Auto-committing via Git
# - Running the kernel in QEMU
# - Validating the ELF binary with `grub-file` for multiboot support
#
# These functions are imported into `build.py` and called based on CLI arguments.
# @author Will
# @date 2025
# @version 1.0
#
# @note This file is part of a larger modular build system.

import subprocess
from bfiles.files import *
import os
from pathlib import Path
from datetime import datetime
from bfiles.toolsflags import *
def fmt():
    os.chdir(Path(__file__).resolve().parents[1])
    subprocess.run(["clang-format", "-i", *SRCS, *HEADS])

def git():
    os.chdir(Path(__file__).resolve().parents[1])
    subprocess.run(["./loc.sh"])
    subprocess.run(["git", "add", "."])
    subprocess.run(["git", "commit", "-m", "Updated at: " + datetime.now().strftime("%H:%M:%S")])
    subprocess.run(["git", "push", "origin", "main"])

def run():
    os.chdir(Path(__file__).resolve().parents[1])
    subprocess.run([QEMU, *QEMUFLAGS])

def chkm():
    os.chdir(Path(__file__).resolve().parents[1])
    try:
        subprocess.run(["i686-elf-grub-file", "--is-x86-multiboot", "kernel.elf"], check=True)
    except subprocess.CalledProcessError:
        print("Invalid multiboot header")
