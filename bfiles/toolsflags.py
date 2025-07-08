import build

# Tools
if build.ARCH == "X86":
	CC = "i686-elf-gcc"
	LD = "i686-elf-ld"
	AS = "nasm"
	GRUB = "i686-elf-grub-mkrescue"
	QEMU = "qemu-system-i386"
elif build.ARCH == "X64":
	CC = "x86_64-elf-gcc"
	LD = "x86_64-elf-ld"
	AS = "nasm"
	GRUB = "x86_64-elf-grub-mkrescue"
	QEMU = "qemu-system-x86_64"
else:
	print("Unsupported ARCHs")

# Flags
if build.ARCH == "X86":
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
elif build.ARCH == "X64":
	# Compiler flags for x86_64 freestanding kernel
	CCFLAGS = [
		"-ffreestanding", "-I./src",
		"-DARCH_X64", "-DVMQEMU", "-fno-pie",           # no position independent executable (kernel)
		"-nostdlib", "-O2","-g", "-fno-omit-frame-pointer",
		"-std=c99",           # C99 works, but C11 is usually better supported for 64-bit
		"-Wall", "-Wextra", "-m64",               # target 64-bit
	]

	# Linker flags for x86_64 ELF 64-bit kernel
	LDFLAGS = [
		"-melf_x86_64",       # ELF 64-bit format
		"-n",                 # no page align for sections (usually)
		"--oformat=elf64-x86-64"
	]

	# Assembler flags for 64-bit ELF
	ASFLAGS = [ "-felf64", "-g", "-Fdwarf" ]

	# QEMU flags usually stay the same unless you want to upgrade machine type or accel
	QEMUFLAGS = [
		"-machine", "q35,accel=tcg",   # q35 is more modern than pc for x64
		"-cpu", "qemu64",                # emulate host cpu for max features
		"-drive", "file=mykern.iso,format=raw,media=cdrom,if=ide,index=2",
		"-drive", "file=fat16.img,format=raw,if=ide,index=0",
		"-boot", "d",
		"-gdb", "tcp::1234",
		"-monitor", "stdio",
		"-qmp", "tcp:localhost:4444,server,nowait",
		"-netdev", "user,id=n1",
		"-device", "virtio-net-pci,netdev=n1"
	]