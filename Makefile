ASM = nasm
CC  = gcc
LD  = ld

ASMFLAGS = -f elf32
CFLAGS   = -m32 -ffreestanding -fno-stack-protector -fno-pic -Wall -Wextra
LDFLAGS  = -m elf_i386 -T link.ld

KERNEL = kernel.elf
ISO    = raef-os.iso

OBJS = \
	loader.o \
	kernel/kmain.o \
	kernel/shell.o \
	drivers/framebuffer.o \
	drivers/keyboard.o \
	lib/string.o

# ===== DEFAULT =====
all: $(ISO)

# ===== ISO =====
$(ISO): $(KERNEL)
	cp $(KERNEL) iso/boot/kernel.elf
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-boot-info-table \
		-o $(ISO) iso

# ===== LINK =====
$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(KERNEL)

# ===== ASM =====
loader.o: loader.s
	$(ASM) $(ASMFLAGS) $< -o $@

# ===== KERNEL =====
kernel/kmain.o: kernel/kmain.c
	$(CC) $(CFLAGS) -Ilib -c $< -o $@

kernel/shell.o: kernel/shell.c
	$(CC) $(CFLAGS) -Ilib -c $< -o $@

# ===== DRIVERS =====
drivers/framebuffer.o: drivers/framebuffer.c
	$(CC) $(CFLAGS) -Ilib -c $< -o $@

drivers/keyboard.o: drivers/keyboard.c
	$(CC) $(CFLAGS) -Ilib -c $< -o $@

# ===== LIB =====
lib/string.o: lib/string.c
	$(CC) $(CFLAGS) -c $< -o $@

# ===== RUN =====
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

# ===== CLEAN =====
clean:
	rm -f $(OBJS) *.elf *.iso
