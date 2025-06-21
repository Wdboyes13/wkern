i686-elf-grub-file --is-x86-multiboot kernel.elf
if [ $? -ne 0 ]; then
    echo "Multiboot Header Invalid"
    exit 1
fi
