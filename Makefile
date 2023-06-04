TARGET_BIN=myos.bin
TARGET_ISO=myos.iso

C_SOURCES=$(shell find ./ -name "*.c")
C_OBJECTS=$(patsubst %.c, %.o, $(C_SOURCES))

S_SOURCES=$(shell find ./ -name "*.s")
S_OBJECTS=$(patsubst %.s, %.o, $(S_SOURCES))

ASM=i386-elf-as

CC=i386-elf-gcc
C_FLAGS=-c -Wall -Wextra -ffreestanding -O2 -std=gnu99

LD=i386-elf-gcc
LD_FLAGS=-ffreestanding -O2 -nostdlib



#
# Targets for building.
#
all: clean $(S_OBJECTS) $(C_OBJECTS)  kernel verify update qemu

$(S_OBJECTS): %.o: %.s
	@echo
	@echo "Compiling kernel assembly '$<'..."
	$(ASM) $(ASM_FLAGS) -o $@ $<

$(C_OBJECTS): %.o: %.c
	@echo
	@echo  "Compiling kernel C code '$<'..."
	$(CC) $(C_FLAGS) -o $@ $<

kernel: $(C_OBJECTS) $(S_OBJECTS)
	@echo
	@echo "Linking kernel image..."  # Remember to link 'libgcc'.
	$(LD) $(LD_FLAGS) -T linker.ld -lgcc -o $(TARGET_BIN) $(S_OBJECTS) $(C_OBJECTS)


#
# Verify GRUB multiboot sanity.
#
.PHONY: verify
verify:
	@if grub-file --is-x86-multiboot $(TARGET_BIN); then \
        echo;                                            \
        echo  "VERIFY MULTIBOOT: Confirmed ✓"; \
    else                                                 \
        echo;                                            \
        echo  "VERIFY MULTIBOOT: FAILED ✗";    \
    fi


#
# Update CDROM image.
#
.PHONY: update
update:
	@echo
	@echo "Writing to CDROM..."
	    cp $(TARGET_BIN) iso/boot/$(TARGET_BIN)
	   grub-mkrescue -o $(TARGET_ISO) iso


#
# Launching QEMU/debugging.
#
.PHONY: qemu
qemu:
	@echo
	@echo "Launching QEMU..."
	qemu-system-i386 -vga std -cdrom $(TARGET_ISO)


#
# Clean the produced files.
#
.PHONY: clean
clean:
	@echo
	@echo "Cleaning the build..."
	rm -f $(S_OBJECTS) $(C_OBJECTS) $(TARGET_BIN) $(TARGET_ISO)
