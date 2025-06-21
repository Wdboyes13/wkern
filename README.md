# WKern  
**This is a kernel/OS I'm working on**  

__The specs of the machine are very important__  
__Since I dont have the energy to port it to working on other machines__  

__THIS WILL ONLY WORK IN THE PRESET QEMU VM CONFIG RUN WITH `make test`__  

## Tools
__Make sure to run the script to check these with `./configure.sh`__  
__For Platform-Specific Info on installing these go to [docs/platform/](/docs/platform/) And Select your OS__   
- `dd` For Making a new disk  
- __A macOS or Linux Computer/VM__  
- `mkfs.fat` Tool - From `dosfstools`
- NASM Assembler  (`nasm`)
- `qemu-system-i386` 
- GNU Make  (`make`)
- `i686-elf-gcc` Cross Compiler  
- `i686-elf-ld` Cross Linker  
- `i686-elf-grub-mkrescue` - For making the ISO __(E-Mail william@weelam.ca if you want me to build it for you)__   
  
>[!NOTE]  
>The CC/GRUB/LD Can be replaced if you have a Compiler that is the same  
>It MUST be for x86 not x86_64  
>To change it edit Make/tools.mk  


## To Run

```sh
./mkdisk.sh       # This creates the disk that the OS will use
make              # This will run ALL Compilation and Tests
make test         # This will run QEMU Using the required config
```

## This project is licensed under the GNU General Public License v3
## For more info, see [`LICENSE`](/LICENSE)
## Or for the File Header [`FileHeader`](/FileHeader)