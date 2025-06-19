NASM=$1
$NASM -v | grep "NASM"
isnasm=$?
if [ isnasm = 1 ]; then
   echo "Assembler is NOT NASM"
   exit 1
fi
   
$NASM ~/coding/wkern/tsts/nasm-tst.asm
nasmworks=$?
if [ nasmworks = 1 ]; then
   echo "NASM Assembler Broken"
   exit 1
fi
