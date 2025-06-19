CC=$1
ARCH=$($CC /Users/william/coding/wkern/tsts/cc-arch-tsr.c -o /Users/william/coding/wkern/tsts/arch.o -c | grep "i386" | echo $?)
if [ $ARCH = 1 ]; then
    echo "Invalid Compiler Architecture"
    exit 1
fi

rm /Users/william/coding/wkern/tsts/arch.o
