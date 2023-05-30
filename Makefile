ROOT := ~/projects/toy-os

SRCS := $(wildcard *.c) $(wildcard */*.c) $(wildcard *.s) $(wildcard */*.s)

COBJS := $(patsubst %.c, %.o, $(filter %.c, $(SRCS)))

SOBJS := $(patsubst %.s, %.o, $(filter %.s, $(SRCS)))

EXECUTABLE := myos.bin

LINKER_SCRIPT := linker.ld

LDFLAGS := -T $(LINKER_SCRIPT) -ffreestanding -O2 -nostdlib -lgcc

CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra

%.o: %.c
	i386-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	i386-elf-as $< -o $@

$(EXECUTABLE): $(COBJS) $(SOBJS)
	i386-elf-gcc $(LDFLAGS) $^ -o $@

copy:
	cp myos.bin $(ROOT)/iso/boot/myos.bin

boot:
	cd $(ROOT)
	grub-mkrescue -o myos.iso iso
	qemu-system-x86_64 -cdrom myos.iso

all: $(EXECUTABLE) copy boot

clean:
	$(RM) $(COBJS) $(SOBJS) $(EXECUTABLE) *.iso $(ROOT)/iso/boot/myos.bin
