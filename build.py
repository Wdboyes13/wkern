#!/usr/bin/env python3

##
# @file build.py
# @brief Entry point for the OS kernel build system.
#
# This script coordinates the entire build process, as well as cleanup,
# running, and testing workflows.
#
# ## Usage:
# - `./build.py` : Configure (if needed) and build the project
# - `./build.py -clean` : Clean build artifacts
# - `./build.py -git` : Clean and perform Git-specific cleanup
# - `./build.py -run` : Run the built image using QEMU
# - `./build.py -test` : Re-run configure.py to verify system setup
# - `./build.py -loc` : Count lines of code via `loc.sh`
# - `./build.py -mkd` : Create and format a blank FAT16 disk image
##

import sys
import os
from bfiles.brules import *
from bfiles.files import *
from bfiles.frules import *
import time

##
# @brief Main build routine.
#
# Calls the build pipeline in order:
# - format output directory
# - compile ASM
# - compile C
# - partial link
# - final link
# - generate ISO
# - check multiboot
def build():
    start_time = time.time()
    fmt()
    compileasm()
    compilec()
    partial_link()
    link()
    iso()
    chkm()
    end_time = time.time()
    elapsed = end_time - start_time
    print(f"\n\033[36mFinished in {elapsed:.2f} seconds\033[0m")

##
# @brief Remove build output files and Python caches.
#
# Deletes files listed in `CLEANTARGS` and `__pycache__` directories.
def clean():
    for file in CLEANTARGS:
        try:
            os.remove(file) 
        except FileNotFoundError:
            pass
    try:
         shutil.rmtree("__pycache__")
    except FileNotFoundError:
         pass
    try:
         shutil.rmtree("bfiles/__pycache__")
    except FileNotFoundError:
         pass
    
##
# @brief CLI entrypoint for build.py
#
# Parses arguments and dispatches to the correct build system action:
# - No args: Build after configuring if necessary
# - `-clean`: Clean build output
# - `-git`: Clean output and call `git()` (likely to prepare for commit)
# - `-run`: Run the kernel via QEMU
# - `-test`: Validate environment with configure.py
# - `-loc`: Run `loc.sh` to count lines of code
# - `-mkd`: Create and format a blank FAT16 image using mkfs.fat
def main():
    if (len(sys.argv) == 1):
        if os.path.isfile('./didconf'):
            build()
        else:
            subprocess.run(["./configure.py"])
            build()
    elif (sys.argv[1] == "-clean"):
            clean()
    elif (sys.argv[1] == "-git"):
            clean()
            git()
    elif (sys.argv[1] == "-run"):
            run()
    elif (sys.argv[1] == "-test"):
        try:
          subprocess.run(["./configure.py"], check=True)
        except subprocess.CalledProcessError as e:
          print(f"Check exited with code {e.returncode}")
          sys.exit(1)
    elif (sys.argv[1] == "-loc"):
         subprocess.run(["./loc.sh"])
    elif (sys.argv[1] == "-mkd"):
         subprocess.run(["dd", "if=/dev/zero", "of=fat16.img", "bs=1M", "count=64"])
         subprocess.run(["mkfs.fat", "-F", "16", "fat16.img"])


# === Script entry point ===
if __name__ == "__main__":
    main()