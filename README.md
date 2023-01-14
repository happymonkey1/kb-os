# kb-os

kbOS is a simple, 32 bit proof-of-concept operating system. This project was done to explore assembly, C, and operating system development.

## Compilation and Running

Required libraries
- nasm
- gcc
- make
- qemu
- grub
  - xorriso
- wsl (windows only)

To compile on linux
```
make clean
make iso
```

To compile on Windows
```
wsl make clean
wsl make iso
```

Note: Ensure `gcc`, `nasm`, `make`, `grub`, and `xorriso` are installed on your wsl image.

Execute `make qemu` to run.

If the os does not boot or you see a CD-rom boot error 0004, run `sudo apt-get install grub-pc-bin`.

## Acknowledgements and Credits

Tutorials and references used
- https://github.com/cfenollosa/os-tutorial/tree/master/
- https://dev.to/frosnerd/writing-my-own-boot-loader-3mld
- https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf
- CD-rom error 0004 solution: https://github.com/microsoft/WSL/issues/1043 (seems like a wsl issue, haven't tested on native linux)
