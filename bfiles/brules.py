import os
import subprocess
import shutil
from bfiles.files import *
from bfiles.toolsflags import *
from pathlib import Path

def compileasm():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())
    for file in NASMSRCS:
        print(f"Assembling {file}")
        subprocess.run([AS, *ASFLAGS, file, "-o", "objs/" + file.replace(".asm", ".o")])

def compilec():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())
    for file in SRCS:
        print(f"Compiling {file}")
        subprocess.run([CC, *CCFLAGS, "-c", file, "-o", "objs/" + file.replace(".c", ".o")])

def link():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())
    print("Linking")
    subprocess.run([LD, *LDFLAGS, "-T", "link.ld", "-o", OUT, *OBJS])

def iso():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())
    print("Making ISO")
    shutil.copy(GRUBCFG, GRUBCFGTARG)
    shutil.copy(OUT, OUTARG)
    subprocess.run([GRUB, "-o", ISO, "iso"])