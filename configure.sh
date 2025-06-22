#!/usr/bin/env bash
set -euo pipefail
trap 'rm -f tmp.c tmp.asm tmp.o disk.img' EXIT

BLUE=$'\033[0;34m'
RED=$'\033[0;31m'
GREEN=$'\033[0;32m'
YELLOW=$'\033[0;33m'
MAGENTA=$'\033[0;35m'
CYAN=$'\033[0;36m'
CLEAR=$'\033[0m'

yes() { 
    echo -e "\033[30;42m YES \033[0m"
}
no()  { 
    echo -e "\033[30;41m NO  \033[0m]"
}

if [ -f conf.mk ] && [ "${1:-}" != "--reconfigure" ]; then
    echo "${RED}Already configured. Run with --reconfigure to force.${CLEAR}"
    exit 0
fi

if [ "${1:-}" = "--reconfigure" ]; then
    rm conf.mk
fi

check_tool() {
    tool="$1"
    echo "${BLUE}Checking for $tool${CLEAR}"
    "$tool" --version >/dev/null 2>&1
    if [ $? -ne 0 ]; then
        no
        exit 1
    else
        yes
    fi
}

check_ld_flag() {
    echo "${RED}Checking for LD Flag \"$*\"${CLEAR}"
    echo 'int main() { return 0; }' > tmp.c
    i686-elf-gcc -c tmp.c -o tmp.o
    if i686-elf-ld "$@" -o /dev/null tmp.o >/dev/null 2>&1; then
        yes
    else
        no
        exit 1
    fi
}

check_cc_flag() {
    echo "${MAGENTA}Checking for CC Flag \"$*\"${CLEAR}"
    echo "int main() { return 0; }" > tmp.c
    if i686-elf-gcc "$@" -c tmp.c -o /dev/null >/dev/null 2>&1; then
        yes
    else
        no
        exit 1
    fi
}

echo "${CYAN}Checking for required tools${CLEAR}"
printf "\n"

check_tool "make"
check_tool "nasm"
check_tool "i686-elf-gcc"
check_tool "i686-elf-ld"
check_tool "i686-elf-grub-mkrescue"
check_tool "qemu-system-i386"
check_tool "dd"

printf "\n"

echo "${CYAN}Checking if mkfs.msdos works${CLEAR}"
dd if=/dev/zero of=disk.img bs=1M count=64 >/dev/null 2>&1
mkfs.msdos disk.img -F 16 >/dev/null 2>&1
if [ $? -ne 0 ]; then
    no
    exit 1
else
    yes
fi

echo "${CYAN}Checking if NASM works${CLEAR}"
cat > tmp.asm <<EOF
_start:
	mov eax, 0x00
	mov ebx, 0x00
	ret
EOF
nasm tmp.asm -o /dev/null >/dev/null 2>&1
if [ $? -ne 0 ]; then
    no
    exit 1
else
    yes
fi

printf "\n"
echo "${CYAN}Checking for compiler flags${CLEAR}"
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

echo "${CYAN}Checking for linker flags${CLEAR}"
printf "\n"
check_ld_flag "-melf_i386"
check_ld_flag "-n"
check_ld_flag "--oformat=elf32-i386"
printf "\n"

echo "CONFIGURED:=1" > conf.mk


if [ ! -d "./objs" ]; then
    echo "${YELLOW}Creating build directory${CLEAR}"
    find src -type d -exec mkdir -p objs/{} \;
fi

echo 'Configured - run `make` to start build'