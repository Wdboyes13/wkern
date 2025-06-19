ISMULTIBOOT=$(i686-elf-grub-file --is-x86-multiboot kernel.elf | echo $?)

if [ $ISMULTIBOOT = 1 ]; then
    echo "Multiboot Header Invalid"
    exit 1
fi
