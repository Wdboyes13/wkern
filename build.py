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
            print("Run ./configure.sh first - or ./build.py -test !")
    elif (sys.argv[1] == "-clean"):
            clean()
    elif (sys.argv[1] == "-git"):
            clean()
            git()
    elif (sys.argv[1] == "-run"):
            run()
    elif (sys.argv[1] == "-test"):
        subprocess.run(["./configure.sh"])

if __name__ == "__main__":
    main()