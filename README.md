

# Raef OS

**by rfff-glitch**

Raef OS is a minimal 32‑bit x86 operating system built from scratch for learning kernel development, low‑level systems programming, and OS internals. It boots via GRUB, runs a freestanding C kernel, and provides a basic text terminal with keyboard input.

---

## Features (Current)

* GRUB (Multiboot) boot
* 32‑bit x86 freestanding C kernel
* VGA text‑mode framebuffer driver

  * Clear screen
  * Cursor handling
  * Backspace
  * Scrolling
* Simple shell

  * Commands: `help`, `clear`, `flex`
* Keyboard input (polling)
* ISO build + QEMU run

---

## Project Structure

```
raef-os/
├── drivers/
│   ├── framebuffer.c
│   └── framebuffer.h
├── kernel/
│   └── kmain.c
├── iso/
│   └── boot/
│       ├── grub/
│       │   ├── menu.lst
│       │   └── stage2_eltorito
│       └── kernel.elf
├── loader.s
├── link.ld
├── Makefile
├── bochsrc.txt
├── run.sh
└── README
```

---

## Requirements

### Build Environment

* Linux or WSL2 (Ubuntu recommended)
* `gcc` with multilib support
* `nasm`
* `ld`
* `genisoimage`
* `qemu-system-i386`

Install on Ubuntu / WSL:

```bash
sudo apt update
sudo apt install -y build-essential gcc-multilib nasm genisoimage qemu-system-x86
```

---

## Build & Run

### Build ISO

```bash
make
```

### Run in QEMU

```bash
make run
```

### Clean build artifacts

```bash
make clean
```

### Full rebuild + run

```bash
make clean && make run
```

---

## Keyboard & Shell Usage

* Type commands at the prompt (`>`)
* Press **Enter** to execute
* **Backspace** works correctly
* Screen scrolls automatically

Available commands:

* `help`  – show help
* `clear` – clear screen
* `flex`  – ASCII art

---

## Technical Notes

* The kernel is **freestanding** (no libc)
* All hardware access is done directly (I/O ports, VGA memory)
* Keyboard input currently uses **polling** (temporary)
* CPU `hlt` is intentionally avoided until IRQs are implemented

---

## Roadmap

Planned next steps:

* IDT + IRQ setup
* Keyboard interrupt (IRQ1)
* Serial logging
* `kprintf` formatted output
* GDT setup
* Paging & memory management
* Process/task system

---

## Goals

Raef OS is **not** intended to be a daily‑use OS.
Its purpose is to:

* Learn OS development
* Understand hardware‑level programming
* Build a clean kernel architecture
* Serve as a systems programming lab

---

## License

Educational / personal project.
Use at your own risk.

---

## Author

**rfff-glitch**

Built low‑level. No shortcuts.
