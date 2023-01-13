# kb-os

kbOS is a simple, 32 bit proof-of-concept operating system. This project was done to explore assembly, C, and operating system development.

## Compilation and Running

Required libraries
- nasm
- gcc
- make
- qemu
- wsl (windows only)

To compile
```
make clean
make iso
```

Note: on windows, preprend wsl to `make clean` and `make iso`. nasm needs to be installed on your wsl installation.

Execute `make qemu` to run

## Acknowledgements and Credits

Tutorials and references used
- https://github.com/cfenollosa/os-tutorial/tree/master/
- https://dev.to/frosnerd/writing-my-own-boot-loader-3mld
- https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf
