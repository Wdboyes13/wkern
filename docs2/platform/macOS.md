# macOS Toolset installation guide  
  
## Prerequesites: 
- Homebrew Package Manager (`brew`) from [brew.sh](https://brew.sh)  
- Access to terminal  
  
## Installation
```sh
brew install i686-elf-binutils i686-elf-gcc i686-elf-grub
brew install make dosfstools nasm qemu
export PATH="/opt/homebrew/opt/make/libexec/gnubin:$PATH"

# Then proceed to building . . .
```