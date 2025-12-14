
ASM     = nasm
CC      = gcc
LD      = ld

# Flags

ASMFLAGS = -f elf32
CFLAGS   = -m32 -ffreestanding -fno-stack-protector -fno-pic -Wall -Wextra
LDFLAGS  = -m elf_i386 -T link.ld

# Files
KERNEL  = kernel.elf
ISO     = raef-os.iso

OBJS = \
	loader.o \
	kmain.o \
	framebuffer.o


# Default target
all: $(ISO)


# Build the ISo
$(ISO): $(KERNEL)
	cp $(KERNEL) iso/boot/kernel.elf
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-boot-info-table \
		-o $(ISO) iso
# Link kernel
$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(KERNEL)

# Compile ASM
loader.o: loader.s
	$(ASM) $(ASMFLAGS) loader.s -o loader.o

# Compile C files
kmain.o: kernel/kmain.c
	$(CC) $(CFLAGS) -c kernel/kmain.c -o kmain.o

framebuffer.o: drivers/framebuffer.c
	$(CC) $(CFLAGS) -c drivers/framebuffer.c -o framebuffer.o

# Run in QEMU
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

# Clean
clean:
	rm -f *.o *.elf *.iso
