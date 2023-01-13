# kb-os
Kablunk operating system

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
