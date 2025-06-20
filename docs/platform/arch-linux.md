# Arch Linux Toolset installation guide  
  
## Prerequesites: 
- Pacman Package Manager (`pacman`)
- Access to root (`sudo`)  
- YAY Package Manager (`yay`)

# Installation
```sh
sudo pacman -S base-devel nasm dosfstools qemu grub make
yay -S i686-elf-binutils i686-elf-gcc
```