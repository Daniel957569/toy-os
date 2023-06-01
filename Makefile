C_SOURCES = $(wildcard *.c) $(wildcard */*.c)
HEADERS = $(wildcard *.h) $(wildcard */*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o} 

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
CFLAGS = -g -m32 -nostdlib -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs  -Wall -Wextra 


myos.bin: boot/bootsect.bin kernel.bin
	cat $^ > myos.bin


kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

run: myos.bin
	qemu-system-i386 -fda myos.bin

debug: myos.bin kernel.elf
	qemu-system-i386 -s -fda myos.bin -d guest_errors,int &
	gdb  myos.bin


%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

all: myos.bin run

clean:
	rm -rf *.bin *.dis *.o myos.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o
