# Raef OS

![License](https://img.shields.io/badge/license-Educational-blue)
![Architecture](https://img.shields.io/badge/arch-x86--32bit-green)
![Language](https://img.shields.io/badge/language-C%20%7C%20Assembly-orange)

> A minimal 32-bit x86 operating system built from scratch for educational purposes

**Raef OS** is a freestanding operating system kernel developed to explore low-level systems programming, OS internals, and x86 architecture. Built without external libraries, it demonstrates fundamental OS concepts including bootloading, memory-mapped I/O, and hardware interaction.

---

## ✨ Features

### Current Implementation

- **🔧 Bootloader Integration**
  - GRUB Multiboot-compliant bootloader
  - Custom assembly loader (`loader.s`)
  - ISO generation for deployment

- **💻 Kernel Capabilities**
  - 32-bit x86 freestanding C kernel
  - No standard library dependencies
  - Direct hardware access via I/O ports

- **🖥️ Display System**
  - VGA text-mode framebuffer driver (80x25)
  - Screen clearing and cursor management
  - Automatic scrolling
  - Backspace functionality

- **⌨️ Input Handling**
  - PS/2 keyboard driver (polling mode)
  - Real-time character input
  - Command-line interface

- **🐚 Simple Shell**
  - Interactive prompt (`>`)
  - Built-in commands:
    - `help` - Display available commands
    - `clear` - Clear the terminal
    - `flex` - ASCII art display
    - `uptime` - Show system uptime since boot
    - `echo` - Echo text back to terminal
    - `reboot` - Restart the operating system

---

## 📁 Project Structure

```
raef-os/
├── drivers/              # Hardware drivers
│   ├── framebuffer.c     # VGA text-mode driver
│   └── framebuffer.h     # Framebuffer interface
├── kernel/               # Kernel source
│   └── kmain.c           # Main kernel entry point
├── lib/                  # Utility libraries
├── iso/                  # ISO build directory
│   └── boot/
│       ├── grub/         # GRUB configuration
│       │   ├── menu.lst  # GRUB menu
│       │   └── stage2_eltorito
│       └── kernel.elf    # Compiled kernel
├── loader.s              # Assembly bootloader
├── link.ld               # Linker script
├── Makefile              # Build automation
├── bochsrc.txt           # Bochs emulator config
├── stage2_eltorito       # GRUB Stage 2 bootloader
└── README.md
```

---

## 🛠️ Requirements

### Build Tools

| Tool | Purpose |
|------|---------|
| `gcc` (with multilib) | C compiler for 32-bit targets |
| `nasm` | Netwide Assembler for x86 |
| `ld` | GNU linker |
| `genisoimage` | ISO 9660 filesystem creation |
| `qemu-system-i386` | x86 emulator for testing |

### Installation

**Ubuntu / Debian / WSL2:**
```bash
sudo apt update
sudo apt install -y build-essential gcc-multilib nasm genisoimage qemu-system-x86
```

**Arch Linux:**
```bash
sudo pacman -S base-devel gcc-multilib nasm cdrtools qemu-system-x86
```

**Fedora / RHEL:**
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install gcc.i686 nasm genisoimage qemu-system-x86
```

---

## 🚀 Quick Start

### Build the OS

```bash
make
```

This compiles the kernel, assembles the loader, and packages everything into a bootable ISO (`kernel.iso`).

### Run in QEMU

```bash
make run
```

Launches the OS in QEMU with optimized settings for development.

### Clean Build Artifacts

```bash
make clean
```

Removes all compiled binaries and intermediate files.

### Full Rebuild

```bash
make clean && make
```

---

## 🎮 Usage

### Interactive Shell

Once booted, you'll see the Raef OS prompt:

```
> _
```

**Available Commands:**

| Command | Description |
|---------|-------------|
| `help` | Display command list and usage information |
| `clear` | Clear the terminal screen |
| `flex` | Display ASCII art logo |
| `uptime` | Show elapsed time since boot |
| `echo [text]` | Print text to the console |
| `reboot` | Restart the operating system |

### Keyboard Controls

- **Type** commands naturally
- **Enter** executes the command
- **Backspace** deletes characters
- Screen automatically scrolls when full

---

## 🔬 Technical Details

### Architecture

- **Target**: x86 (IA-32)
- **Boot Protocol**: Multiboot
- **Environment**: Freestanding (no hosted C library)
- **Linking**: Custom linker script (`link.ld`)

### Memory Layout

```
0x00100000 (1MB)  - Kernel load address
0x000B8000        - VGA text buffer (80x25x2 bytes)
```

### Hardware Access

- **VGA**: Memory-mapped I/O at `0xB8000`
- **Keyboard**: Port `0x60` (data) and `0x64` (status)
- **Direct I/O**: No abstraction layers

### Build Process

1. **Assembly**: `nasm` assembles `loader.s` to `loader.o`
2. **Compilation**: `gcc` compiles C sources with `-m32 -ffreestanding`
3. **Linking**: `ld` creates `kernel.elf` using custom script
4. **ISO Creation**: `genisoimage` packages kernel with GRUB

---

## 🗺️ Roadmap

### Phase 1: Core Infrastructure ✅
- [x] Bootloader integration
- [x] Basic kernel setup
- [x] VGA driver
- [x] Keyboard input

### Phase 2: Interrupts & IRQs 🔄
- [ ] Interrupt Descriptor Table (IDT)
- [ ] Programmable Interrupt Controller (PIC) setup
- [ ] Keyboard interrupt handler (IRQ1)
- [ ] Timer interrupt (IRQ0)

### Phase 3: Enhanced I/O
- [ ] Serial port driver (COM1)
- [ ] `kprintf` formatted output
- [ ] Logging system

### Phase 4: Memory Management
- [ ] Global Descriptor Table (GDT)
- [ ] Paging implementation
- [ ] Physical memory manager
- [ ] Virtual memory allocator

### Phase 5: Advanced Features
- [ ] Process/task management
- [ ] Filesystem support
- [ ] User mode transition
- [ ] System calls

### Phase 6: User Experience
- [ ] File manager interface
- [ ] Extended shell commands
- [ ] Multi-tasking support

---

## 📚 Learning Resources

This project was built using knowledge from:

- [OSDev Wiki](https://wiki.osdev.org/)
- [Intel® 64 and IA-32 Architectures Software Developer Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
- [Writing a Simple Operating System from Scratch](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)
- [JamesM's Kernel Development Tutorials](http://www.jamesmolloy.co.uk/tutorial_html/)

---

## 🐛 Known Limitations

- **Polling-based keyboard**: IRQ implementation pending
- **No interrupt handling**: Currently uses busy-wait loops
- **Single-threaded**: No multitasking support yet
- **No filesystem**: Data is hardcoded in kernel
- **Limited error handling**: Minimal validation and recovery

---

## 🎯 Project Goals

This OS is **not** intended for production use. It serves as:

- 🎓 An educational platform for OS development
- 🔧 A testbed for low-level programming concepts
- 🧪 A learning environment for x86 architecture
- 💡 A foundation for understanding kernel internals

---

## 🤝 Contributing

While this is primarily a personal learning project, suggestions and feedback are welcome! Feel free to:

- Open issues for bugs or questions
- Suggest improvements or features
- Share your own OS development experiences

---

## 📜 License

This project is for **educational purposes** only. Use at your own risk.

No warranty is provided. The code is provided as-is for learning and experimentation.

---

## 👨‍💻 Author

**rff-glitch**

*Built from the ground up. No shortcuts. No regrets.*

---

## 🙏 Acknowledgments

Special thanks to the OS development community for their extensive documentation and resources that made this project possible.

---

<div align="center">

**Made with ☕ and 💻**

*"Understanding how computers work by building one from scratch"*

</div>
