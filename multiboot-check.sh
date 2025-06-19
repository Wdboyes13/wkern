ISMULTIBOOT=xxd -l 16 kernel.bin | grep "02b0 ad1b" | echo $?

if [ $ISMULTIBOOT = 1 ]; then
    echo "Multiboot Header Invalid"
    exit 1
fi
