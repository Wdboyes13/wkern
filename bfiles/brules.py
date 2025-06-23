import os
import subprocess
import shutil
import sys
from bfiles.files import *
from bfiles.toolsflags import *
from pathlib import Path

def compileasm():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())
    for file in NASMSRCS:
        print(f"Assembling {file}")
        try:
            subprocess.run([AS, *ASFLAGS, file, "-o", "objs/" + file.replace(".asm", ".o")], check=True)
        except subprocess.CalledProcessError as e:
            print(f"Failed with code {e.returncode}", file=sys.stderr)
            sys.exit(1)
def compilec():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())
    for file in SRCS:
        print(f"Compiling {file}")
        try:
            subprocess.run([CC, *CCFLAGS, "-c", file, "-o", "objs/" + file.replace(".c", ".o")], check=True)
        except subprocess.CalledProcessError as e:
            print(f"Failed with code {e.returncode}", file=sys.stderr)
            sys.exit(1)

def link():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())
    print("Linking")
    try:
        subprocess.run([LD, *LDFLAGS, "-T", "link.ld", "-o", OUT, *OBJS], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Failed with code {e.returncode}", file=sys.stderr)
        sys.exit(1)

def iso():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())
    print("Making ISO")
    shutil.copy(GRUBCFG, GRUBCFGTARG)
    shutil.copy(OUT, OUTARG)
    subprocess.run([GRUB, "-o", ISO, "iso"])