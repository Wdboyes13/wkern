NASM=$1
isnasm=$($NASM -v | grep "NASM" | echo $?)
if [ isnasm = 1 ]; then
   echo "Assembler is NOT NASM"
   exit 1
fi
   
$NASM /Users/william/coding/wkern/tsts/nasm-tst.asm
nasmworks=$?
if [ nasmworks = 1 ]; then
   echo "NASM Assembler Broken"
   exit 1
fi

rm /Users/william/coding/wkern/tsts/nasm-tst
