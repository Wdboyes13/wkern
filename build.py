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

def main():
    if os.path.isfile('./didconf'):
        if (len(sys.argv) == 1):
            build()
        elif (sys.argv[1] == "-clean"):
            clean()
        elif (sys.argv[1] == "-git"):
            clean()
            git()
        elif (sys.argv[1] == "-run"):
            run()
    elif (len(sys.argv) == 2 and sys.argv[1] == "-test"):
        subprocess.run(["./configure.sh"])
    else:
        print("Run ./configure.sh first - or ./build.py -test !")

if __name__ == "__main__":
    main()