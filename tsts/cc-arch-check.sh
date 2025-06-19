CC=$1
ARCH=$($CC cc-arch-tsr.c -o arch.o -c | grep "i386" | echo $?)
if [ $ARCH = 1 ]; then
    echo "Invalid Compiler Architecture"
    exit 1
fi

rm arch.o
