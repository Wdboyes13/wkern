##
# @file brules.py
# @brief Core build functions: compile, link, ISO creation.
#
# This file includes logic for:
# - Compiling assembly and C sources
# - Partial linking of modules
# - Full kernel linking
# - ISO generation using GRUB
#
# This is used by `build.py` to perform all low-level build operations.
##

import os
import subprocess
import shutil
import sys
from bfiles.files import *         # Source file lists and structure
from bfiles.toolsflags import *    # Compiler/linker tools and flags
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor, as_completed

##
# @brief Compiles all NASM assembly files in `NASMSRCS`.
#
# Skips files that are up to date (i.e. object file newer than source).
# Assembles using `AS` and `ASFLAGS`.
#
# On error, exits the script with a non-zero status.
def compileasm():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())
    for file in NASMSRCS:
        obj = "objs/" + file.replace(".asm", ".o")
        src_time = os.path.getmtime(file)
        obj_time = os.path.getmtime(obj) if os.path.exists(obj) else -1

        if src_time <= obj_time:
            continue

        print(f"Assembling {file}")
        try:
            subprocess.run([AS, *ASFLAGS, file, "-o", obj], check=True)
        except subprocess.CalledProcessError as e:
            print(f"Failed with code {e.returncode}", file=sys.stderr)
            sys.exit(1)

##
# @brief Compile a single C file if out of date.
#
# @param file The `.c` file path to compile
# @return Compilation result message
def compile_file(file):
    obj = "objs/" + file.replace(".c", ".o")
    src_time = os.path.getmtime(file)
    obj_time = os.path.getmtime(obj) if os.path.exists(obj) else -1

    if src_time <= obj_time:
        return "0"  # Indicates no work needed

    print(f"Compiling {file}")
    try:
        subprocess.run([CC, *CCFLAGS, "-c", file, "-o", obj], check=True)
        return f"Compiled {file}"
    except subprocess.CalledProcessError as e:
        return f"Failed {file} with code {e.returncode}"

##
# @brief Compiles all C sources in parallel from `SRCS`.
#
# Uses a thread pool to speed up compilation across multiple cores.
# Exits on the first compilation error.
def compilec():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())

    with ThreadPoolExecutor(max_workers=8) as executor:
        futures = {executor.submit(compile_file, f): f for f in SRCS}

        for future in as_completed(futures):
            result = future.result()
            if result.startswith("Failed"):
                print(result, file=sys.stderr)
                sys.exit(1)
            if result != "0":
                print(result)

##
# @brief Performs partial linking on modules from `MODULES`.
#
# Links grouped `.o` files into monolithic `.o` modules in `objs/modules/`.
# Skips linking if the output is newer than all inputs.
#
# On failure, exits the script.
def partial_link():
    os.chdir(Path(__file__).resolve().parents[1])
    os.makedirs("objs/modules", exist_ok=True)

    for modname, files in MODULES.items():
        obj_files = [f"objs/{src.replace('.c', '.o')}" for src in files]
        output = f"objs/modules/{modname}.o"

        if os.path.exists(output):
            out_time = os.path.getmtime(output)
            if all(os.path.exists(f) and os.path.getmtime(f) <= out_time for f in obj_files):
                print(f"Module {modname} is up to date")
                continue

        print(f"Linking module: {modname}")
        try:
            subprocess.run(["i686-elf-ld", "-r", "-o", output, *obj_files], check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error linking module {modname}: {e}", file=sys.stderr)
            sys.exit(1)

##
# @brief Links the final kernel ELF binary from object files.
#
# Only performs linking if the output file is older than the inputs.
# On failure, exits with an error.
def link():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())

    if os.path.exists(OUT):
        out_time = os.path.getmtime(OUT)
        if all(os.path.exists(obj) and os.path.getmtime(obj) <= out_time for obj in OBJS):
            print("Kernel is up to date")
            return

    print("Linking kernel")
    try:
        subprocess.run(["i686-elf-ld", *LDFLAGS, "-T", "link.ld", "-o", OUT, *OBJS], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Final link failed: {e}", file=sys.stderr)
        sys.exit(1)

##
# @brief Generates a bootable ISO using GRUB and kernel ELF.
#
# Only updates the ISO if the kernel is newer than the ISO file.
# Copies kernel and GRUB config to target locations and invokes `grub-mkrescue`.
def iso():
    os.chdir(Path(__file__).resolve().parents[1])
    src_time = os.path.getmtime("kernel.elf")
    obj_time = os.path.getmtime(ISO) if os.path.exists(ISO) else -1
    if src_time <= obj_time:
        return "0"  # ISO is already up to date

    print("Working from:", os.getcwd())
    print("Making ISO")
    shutil.copy(GRUBCFG, GRUBCFGTARG)
    shutil.copy(OUT, OUTARG)
    subprocess.run(
        [GRUB, "-o", ISO, "iso"],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )