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
    subprocess.run(["git", "add", "."])
    subprocess.run(["git", "commit", "-m", "Updated at: " + datetime.now().strftime("%H:%M:%S")])
    subprocess.run(["git", "push", "origin", "main"])

def run():
    os.chdir(Path(__file__).resolve().parents[1])
    subprocess.Popen([QEMU, *QEMUFLAGS])

def chkm():
    os.chdir(Path(__file__).resolve().parents[1])
    try:
        subprocess.run(["i686-elf-grub-file", "--is-x86-multiboot", "kernel.elf"], check=True)
    except subprocess.CalledProcessError:
        print("Invalid multiboot header")
