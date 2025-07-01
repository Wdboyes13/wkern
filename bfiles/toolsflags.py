# Tools
CC = "i686-elf-gcc"
LD = "i686-elf-ld"
AS = "nasm"
GRUB = "i686-elf-grub-mkrescue"
QEMU = "qemu-system-i386"

# Flags
CCFLAGS = ["-ffreestanding", "-I./src", "-DARCH_X86", "-DVMQEMU", "-fno-pie", "-nostdlib",
           "-m32", "-O2", "-g", "-fno-omit-frame-pointer", "-std=c99", "-Wall", "-Wextra"]

LDFLAGS = ["-melf_i386", "-n", "--oformat=elf32-i386"]

ASFLAGS = ["-felf32", "-g", "-Fdwarf"]

QEMUFLAGS = ["-machine", "pc,accel=tcg",
				   "-drive", "file=mykern.iso,format=raw,media=cdrom,if=ide,index=2",
				   "-drive", "file=fat16.img,format=raw,if=ide,index=0",
				   "-boot", "d", "-gdb", "tcp::1234", "-monitor", "stdio",
				   "-qmp", "tcp:localhost:4444,server,nowait", 
                   "-netdev", "user,id=n1", "-device", "virtio-net-pci,netdev=n1"]