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

def compilec():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())
    for file in SRCS:
        obj = "objs/" + file.replace(".c", ".o")
        src_time = os.path.getmtime(file)
        obj_time = os.path.getmtime(obj) if os.path.exists(obj) else -1

        if src_time <= obj_time:
            continue

        print(f"Compiling {file}")
        try:
            subprocess.run([CC, *CCFLAGS, "-c", file, "-o", obj], check=True)
        except subprocess.CalledProcessError as e:
            print(f"Failed with code {e.returncode}", file=sys.stderr)
            sys.exit(1)

def link():
    os.chdir(Path(__file__).resolve().parents[1])
    print("Working from:", os.getcwd())

    # Check timestamps
    if os.path.exists(OUT):
        out_time = os.path.getmtime(OUT)
        needs_link = any(os.path.getmtime(obj) > out_time for obj in OBJS if os.path.exists(obj))
        if not needs_link:
            return

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
    subprocess.run([GRUB, "-o", ISO, "iso"],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL)