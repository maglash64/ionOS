
kernel_link: kernel_compile
	ld -T kernel/link.ld -m elf_i386 -nostdinc -nostdlib build/kernel.asm.o build/kernel.c.o \
	-o  build/image.bin

kernel_compile: kernel_assemble
	gcc -c kernel/kernel.c -o build/kernel.c.o -m32 -nostdlib -fno-builtin -fno-stack-protector \
	-nostartfiles -ffreestanding -nodefaultlibs  -std=gnu99 -Wall -Wextra  \
	-Os -fstrength-reduce -fomit-frame-pointer -Wl,--build-id=none -no-pie

kernel_assemble:
	nasm -felf32 kernel/kernel.asm -o build/kernel.asm.o
