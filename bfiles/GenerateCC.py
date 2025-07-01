#!/usr/bin/env python3

import os
import json
from bfiles.toolsflags import *
from pathlib import Path

# Root directory to search .c files
ROOT_DIR = "."  # Change if needed

def find_c_files(root):
    c_files = []
    for dirpath, _, filenames in os.walk(root):
        for f in filenames:
            if f.endswith(".c"):
                full_path = os.path.relpath(os.path.join(dirpath, f), root)
                c_files.append(full_path)
    return c_files

def main():
    os.chdir(Path(__file__).resolve().parents[1])
    files = find_c_files(ROOT_DIR)
    compile_commands = []

    for file in files:
        cmd = {
            "directory": os.path.abspath(ROOT_DIR),
            "command": "clang " + " ".join(CCFLAGS) + f" -c {file} -o {file}.o",
            "file": os.path.abspath(file),
        }
        compile_commands.append(cmd)

    with open("compile_commands.json", "w") as f:
        json.dump(compile_commands, f, indent=2)

    print(f"Generated compile_commands.json with {len(files)} entries.")

if __name__ == "__main__":
    main()