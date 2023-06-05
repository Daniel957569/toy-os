TARGET_BIN=myos.bin
TARGET_ISO=myos.iso
TARGET_SYM=myos.sym

C_SOURCES=$(shell find ./ -name "*.c")
C_OBJECTS=$(patsubst %.c, %.o, $(C_SOURCES))

S_SOURCES=$(shell find ./ -name "*.s")
S_OBJECTS=$(patsubst %.s, %.o, $(S_SOURCES))

PREFIX := /usr/local/i386elfgcc
TARGET := i386-elf
PATH := $(PREFIX)/bin:$(PATH)

# Export variables
export PREFIX
export TARGET
export PATH

ASM=i386-elf-as

OBJCOPY=i386-elf-objcopy
OBJDUMP=i386-elf-objdump

CC=i386-elf-gcc
C_FLAGS=-c -Wall -Wextra -ffreestanding -O2 -std=gnu99 -Wno-tautological-compare \
        -g -fno-omit-frame-pointer -fstack-protector

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
	$(LD) $(LD_FLAGS) -T linker.ld -lgcc -o $(TARGET_BIN) \
	-Wl,--oformat,elf32-i386 $(S_OBJECTS) $(C_OBJECTS)
	$(OBJCOPY) --only-keep-debug $(TARGET_BIN) $(TARGET_SYM)
	$(OBJCOPY) --strip-debug $(TARGET_BIN)


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


#debug with gdb
.PHONY: qemu-debug
qemu-debug:
	@echo $(HUX_MSG) "Launching QEMU (debug mode)..."
	qemu-system-i386 -vga std -S -s -cdrom $(TARGET_ISO)

.PHONY: gdb
gdb:
	@echo $(HUX_MSG) "Launching gdb (debug mode)..."
	gdb -ex "target remote localhost:1234" -ex "symbol-file myos.sym"


#
# Clean the produced files.
#
.PHONY: clean
clean:
	@echo
	@echo "Cleaning the build..."
	rm -f $(S_OBJECTS) $(C_OBJECTS) $(TARGET_BIN) $(TARGET_ISO)
