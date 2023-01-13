kernel := build/kernel.bin
iso := build/kbOS.iso

linker_script := linker.ld
grub_cfg := boot/grub.cfg

CFLAGS = -fno-pic -static -fno-builtin -fno-strict-aliasing -O1 -Wall -MD -ggdb -m32 -fno-omit-frame-pointer -Werror -ffreestanding
#CFLAGS = -fno-pic -static -fno-strict-aliasing -O1 -Wall -MD -ggdb -m32 -fno-omit-frame-pointer -Werror

target ?= kbOS

assembly_source_files := $(wildcard boot/multiboot_header.asm boot/kernel_entry.asm *.asm cpu/*.asm driver/*.asm boot/boot.asm)
assembly_object_files := $(patsubst %.asm, build/%.o, $(assembly_source_files))
c_source_files := $(wildcard *.c cpu/*.c driver/*.c kernel/*.c)
c_object_files := $(patsubst %.c, build/%.o, $(c_source_files))

# from https://stackoverflow.com/a/52016618
# detect if running under unix by finding 'rm' in $PATH :
ifeq ($(wildcard $(addsuffix /rm,$(subst :, ,$(PATH)))),)
WINMODE=1
else
WINMODE=0
endif

ifeq ($(WINMODE),1)
# native windows setup :
UNLINK = del $(subst /,\,$(1))
CAT = type $(subst /,\,$(1))
else
# cross-compile setup :
UNLINK = $(RM) $(1)
CAT = cat $(1)
endif

.PHONY: all clean qemu qemu-nox qemu-gdb qemu-gdb-nox

all: $(kernel)

clean:
		rm -rf build

qemu: $(iso)
		qemu-system-i386 -cdrom $(iso) -vga std -s -serial file:serial.log -no-reboot -d guest_errors,int -D log.log

qemu-64: $(iso)
		qemu-system-x86_64 -cdrom $(iso) -vga std -s -serial file:serial.log -no-reboot -d guest_errors,int -D log.log 

qemu-nox-64: $(iso)
		qemu-system-x86_64 -m 128 -cdrom $(iso) -vga std -s -no-reboot -nographic 

qemu-gdb-64: $(iso)
		qemu-system-x86_64 -S -m 128 -cdrom $(iso) -vga std -s -serial file:serial.log -no-reboot -no-shutdown -d int,cpu_reset 

.PHONY: qemu-gdb-nox
qemu-gdb-nox-64: $(iso)
		qemu-system-x86_64 -S -m 128 -cdrom $(iso) -vga std -s -serial file:serial.log -no-reboot -no-shutdown -d int,cpu_reset -nographic

iso: $(iso)
		@echo "Done"

$(iso): $(kernel) $(grub_cfg)
		@mkdir -p build/isofiles/boot/grub
		cp $(kernel) build/isofiles/boot/kernel.bin
		cp $(grub_cfg) build/isofiles/boot/grub
		grub-mkrescue -o $(iso) build/isofiles #2> /dev/null
		@rm -r build/isofiles
        
$(kernel): $(c_object_files) $(assembly_object_files) $(linker_script)
		ld -m elf_i386 -T $(linker_script) -o $(kernel) $(assembly_object_files) $(c_object_files)

# compile C files
build/%.o: %.c
		@mkdir -p $(shell dirname $@)
		gcc $(CFLAGS) -c $< -o $@

# compile assembly files
build/%.o: %.asm
		@mkdir -p $(shell dirname $@)
		nasm -f elf32 $< -o $@

.PHONY: default build run clean