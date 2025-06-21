#!/usr/bin/env bash
set -euo pipefail
trap 'rm -f tmp.c tmp.asm tmp.o disk.img' EXIT

check_tool() {
    tool="$1"
    echo "Checking for $tool"
    "$tool" --version >/dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo "No"
        exit 1
    else
        echo "Yes"
    fi
}

check_ld_flag() {
    echo "Checking for LD Flag $*"
    echo 'int main() { return 0; }' > tmp.c
    i686-elf-gcc -c tmp.c -o tmp.o
    if i686-elf-ld "$@" -o /dev/null tmp.o >/dev/null 2>&1; then
        echo "Yes"
        rm -f tmp.o tmp.c
    else
        echo "No"
        rm -f tmp.o tmp.c
        exit 1
    fi
}

check_cc_flag() {
    echo "Checking for CC Flag $*"
    echo "int main() { return 0; }" > tmp.c
    if i686-elf-gcc "$@" -c tmp.c -o /dev/null >/dev/null 2>&1; then
        echo "Yes"
        rm -f tmp.c
    else
        echo "No"
        rm -f tmp.c
        exit 1
    fi
}

echo "Checking for required tools"
printf "\n"

check_tool "make"
check_tool "nasm"
check_tool "i686-elf-gcc"
check_tool "i686-elf-ld"
check_tool "i686-elf-grub-mkrescue"
check_tool "qemu-system-i386"
check_tool "dd"

printf "\n"

echo "Checking if mkfs.msdos works"
dd if=/dev/zero of=disk.img bs=1M count=64 >/dev/null 2>&1
mkfs.msdos disk.img -F 16 >/dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "No"
    rm -f disk.img
    exit 1
else
    echo "Yes"
    rm -f disk.img
fi

echo "Checking if NASM works"
cat > tmp.asm <<EOF
_start:
	mov eax, 0x00
	mov ebx, 0x00
	ret
EOF
nasm tmp.asm -o /dev/null >/dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "No"
    rm -f tmp.asm
    exit 1
else
    echo "Yes"
    rm -f tmp.asm
fi

printf "\n"
echo "Checking for compiler flags"
printf "\n"
check_cc_flag "-ffreestanding"
check_cc_flag "-nostdlib"
check_cc_flag "-fno-pie"
check_cc_flag "-m32"
check_cc_flag "-O2"
check_cc_flag "-fno-omit-frame-pointer"
check_cc_flag "-std=c99"
check_cc_flag "-g"
printf "\n"

echo "Checking for linker flags"
printf "\n"
check_ld_flag "-melf_i386"
check_ld_flag "-n"
check_ld_flag "--oformat=elf32-i386"
printf "\n"

echo "CONFIGURED:=1" > conf.mk


if [ ! -d "./objs" ]; then
    echo "Creating build directory"
    mkdir -p ./objs/src
    mkdir -p ./objs/src/err
    mkdir -p ./objs/src/fileio/ops
    mkdir -p ./objs/src/idt/handlers
    mkdir -p ./objs/src/io
    mkdir -p ./objs/src/KShell
    mkdir -p ./objs/src/mem
    mkdir -p ./objs/src/qemu
    mkdir -p ./objs/src/types
    mkdir -p ./objs/src/utils
    mkdir -p ./objs/src/wex
fi

echo 'Configured - run `make` to start build'