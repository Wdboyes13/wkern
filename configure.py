#!/usr/bin/env python3

##
# @file configure.py
# @brief Pre-build configuration checker for the kernel build system.
#
# This script checks for required tools, verifies compiler and linker flags, sets up
# directories, clones needed dependencies, and prepares the environment for building.
#
# Run this script before running the actual build (`./build.py`).
# Use `--reconfigure` to force regeneration of the `didconf` file.
##

import os
import sys
import subprocess
import atexit
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import time

# === ANSI Color Constants ===
BLUE    = "\033[0;34m"
RED     = "\033[0;31m"
GREEN   = "\033[0;32m"
YELLOW  = "\033[0;33m"
MAGENTA = "\033[0;35m"
CYAN    = "\033[0;36m"
CLEAR   = "\033[0m"

# === Temporary Files to Clean on Exit ===
tmp_files = ["tmp.c", "tmp.asm", "tmp.o", "disk.img"]
atexit.register(lambda: [os.remove(f) for f in tmp_files if os.path.exists(f)])

##
# @brief Prints a "YES" message in green.
def yes(): print(f"\033[30;42m YES \033[0m")

##
# @brief Prints a "NO" message in red.
def no():  print(f"\033[30;41m NO  \033[0m")

##
# @brief Runs a shell command and checks its success.
#
# @param desc Description of the check
# @param cmd Shell command to run
# @param fatal If True, exit on failure
def run_check(desc, cmd, fatal=True):
    print(desc)
    result = subprocess.run(cmd, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    if result.returncode == 0:
        yes()
    else:
        no()
        if fatal:
            sys.exit(1)

##
# @brief Checks if a given tool is available in PATH.
#
# @param tool Name of the tool (e.g. `make`, `nasm`, etc.)
def check_tool(tool):
    result = subprocess.run(f"{tool} --version", shell=True,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    status = f"\033[30;42m YES \033[0m" if result.returncode == 0 else f"\033[30;41m NO  \033[0m"
    print(f"{BLUE}Checking for {tool}{CLEAR} -> {status}")
    if result.returncode != 0:
        sys.exit(1)

##
# @brief Checks if a compiler flag is accepted by `i686-elf-gcc`.
#
# @param flag The compiler flag to test (e.g. `-ffreestanding`)
def check_cc_flag(flag):
    with open("tmp.c", "w") as f:
        f.write("int main() { return 0; }")
    result = subprocess.run(f"i686-elf-gcc {flag} -c tmp.c -o /dev/null", shell=True,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    status = f"\033[30;42m YES \033[0m" if result.returncode == 0 else f"\033[30;41m NO  \033[0m"
    print(f"{MAGENTA}Checking CC Flag \"{flag}\"{CLEAR} -> {status}")
    if result.returncode != 0:
        sys.exit(1)

##
# @brief Checks if a linker flag is accepted by `i686-elf-ld`.
#
# @param flag The linker flag to test (e.g. `--oformat=elf32-i386`)
def check_ld_flag(flag):
    with open("tmp.c", "w") as f:
        f.write("int main() { return 0; }")
    subprocess.run("i686-elf-gcc -c tmp.c -o tmp.o", shell=True, check=True)
    result = subprocess.run(f"i686-elf-ld {flag} -o /dev/null tmp.o", shell=True,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    status = f"\033[30;42m YES \033[0m" if result.returncode == 0 else f"\033[30;41m NO  \033[0m"
    print(f"{RED}Checking LD Flag \"{flag}\"{CLEAR} -> {status}")
    if result.returncode != 0:
        sys.exit(1)

# === Main Entrypoint ===
if __name__ == "__main__":
    start_time = time.time()
    reconfigure = len(sys.argv) > 1 and sys.argv[1] == "--reconfigure"

    # Check if already configured
    if Path("didconf").exists() and not reconfigure:
        print(f"{RED}Already configured. Run with --reconfigure to force.{CLEAR}")
        sys.exit(0)
    elif reconfigure:
        Path("didconf").unlink(missing_ok=True)

    # Tool Check
    print(f"{CYAN}Checking for required tools{CLEAR}\n")
    tools = ["make", "nasm", "i686-elf-gcc", "i686-elf-ld", "i686-elf-grub-mkrescue", "qemu-system-i386"]
    with ThreadPoolExecutor() as exec:
        exec.map(check_tool, tools)

    # mkfs.msdos test
    print(f"\n{CYAN}Checking if mkfs.msdos works{CLEAR}")
    subprocess.run("dd if=/dev/zero of=disk.img bs=1M count=64", shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    run_check("", "mkfs.msdos disk.img -F 16")

    # NASM test
    print(f"{CYAN}Checking if NASM works{CLEAR}")
    with open("tmp.asm", "w") as f:
        f.write("_start:\n\tmov eax, 0x00\n\tmov ebx, 0x00\n\tret\n")
    run_check("", "nasm tmp.asm -o /dev/null")

    # Compiler flags
    print(f"\n{CYAN}Checking compiler flags{CLEAR}\n")
    cc_flags = ["-ffreestanding", "-nostdlib", "-fno-pie", "-m32", "-O2", "-fno-omit-frame-pointer", "-std=c99", "-g"]
    with ThreadPoolExecutor() as exec:
        exec.map(check_cc_flag, cc_flags)

    # Linker flags
    print(f"\n{CYAN}Checking linker flags{CLEAR}\n")
    ld_flags = ["-melf_i386", "-n", "--oformat=elf32-i386"]
    with ThreadPoolExecutor() as exec:
        exec.map(check_ld_flag, ld_flags)

    # Mark configuration done
    Path("didconf").write_text("Y")

    # Create `objs/` directory structure
    if not Path("./objs").exists():
        print(f"{YELLOW}Creating build directory{CLEAR}")
        for subdir in Path("src").rglob("*"):
            if subdir.is_dir():
                (Path("objs") / subdir).mkdir(parents=True, exist_ok=True)

    # Clone SLRE regex engine if not present
    if not Path("./src/slre").exists():
        subprocess.run("git clone --depth 1 https://github.com/Wdboyes13/slre src/slre", shell=True, check=True)

    end_time = time.time()
    elapsed = end_time - start_time
    print(f"\n\033[36mFinished in {elapsed:.2f} seconds\033[0m")
    print(f"{GREEN}Configured - run `./build.py` to start build{CLEAR}")