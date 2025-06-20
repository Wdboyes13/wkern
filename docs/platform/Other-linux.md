# Linux Toolset installation guide  
  
## Prerequesites: 
- Homebrew Package Manager (`brew`) from [brew.sh](https://brew.sh)  
- Access to terminal  
  
## Installation
```sh
brew install i686-elf-binutils
brew install i686-elf-gcc
brew install i686-elf-grub
brew install make
export PATH="/opt/homebrew/opt/make/libexec/gnubin:$PATH"
brew install nasm
brew install dosfstools
brew install qemu
```