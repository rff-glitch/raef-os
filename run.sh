# Assemble bootloader
nasm -f elf32 loader.s -o loader.o

# Compile kernel and framebuffer separately
gcc -m32 -ffreestanding -c drivers/framebuffer.c -o framebuffer.o
gcc -m32 -ffreestanding -c kernel/kmain.c -o kmain.o

# Link everything into kernel.elf
ld -m elf_i386 -T link.ld loader.o kmain.o framebuffer.o -o kernel.elf




cp kernel.elf iso/boot/
genisoimage -R \
-b boot/grub/stage2_eltorito \
-no-emul-boot \
-boot-load-size 4 \
-boot-info-table \
-o raef-os.iso \
iso

qemu-system-i386 -cdrom raef-os.iso
