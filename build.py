#!/usr/bin/env python3
import sys
import os
from bfiles.brules import *
from bfiles.files import *
from bfiles.frules import *
def build():
    fmt()
    compileasm()
    compilec()
    link()
    iso()
    chkm()

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
    
def main():
    if (len(sys.argv) == 1):
        if os.path.isfile('./didconf'):
            build()
        else:
            subprocess.run(["./configure.sh"])
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
          subprocess.run(["./configure.sh"], check=True)
        except subprocess.CalledProcessError as e:
          print(f"Check exited with code {e.returncode}")
          sys.exit(1)
    elif (sys.argv[1] == "-loc"):
         subprocess.run(["tokei", "--num-format=commas", "-C"])
    elif (sys.argv[1] == "-mkd"):
         subprocess.run(["dd", "if=/dev/zero", "of=fat16.img", "bs=1M", "count=64"])
         subprocess.run(["mkfs.fat", "-F", "16", "fat16.img"])

if __name__ == "__main__":
    main()