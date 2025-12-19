# Tools
ASM     = nasm
CC      = gcc
LD      = ld

# Flags
ASMFLAGS = -f elf32
CFLAGS   = -m32 -ffreestanding -fno-stack-protector -fno-pic -Wall -Wextra
LDFLAGS  = -m elf_i386 -T link.ld

# Output
KERNEL  = kernel.elf
ISO     = raef-os.iso

# Object files
OBJS = \
	loader.o \
	kernel/kmain.o \
	kernel/shell.o \
	drivers/framebuffer.o \
	drivers/keyboard.o

# Default
all: $(ISO)

# ISO
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

# ASM
loader.o: loader.s
	$(ASM) $(ASMFLAGS) loader.s -o loader.o

# Kernel
kernel/kmain.o: kernel/kmain.c
	$(CC) $(CFLAGS) -c kernel/kmain.c -o kernel/kmain.o

kernel/shell.o: kernel/shell.c
	$(CC) $(CFLAGS) -c kernel/shell.c -o kernel/shell.o

# Drivers
drivers/framebuffer.o: drivers/framebuffer.c
	$(CC) $(CFLAGS) -c drivers/framebuffer.c -o drivers/framebuffer.o

drivers/keyboard.o: drivers/keyboard.c
	$(CC) $(CFLAGS) -c drivers/keyboard.c -o drivers/keyboard.o

# Run
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

# Clean
clean:
	rm -f kernel/*.o drivers/*.o *.o *.elf *.iso