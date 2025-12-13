#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((unsigned short*)0xB8000)

static int cursor_x = 0;
static int cursor_y = 0;
static unsigned char color = 0x0F;

static unsigned short vga_entry(char c, unsigned char color) {
    return (unsigned short)c | (unsigned short)color << 8;
}

void fb_clear() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[y * VGA_WIDTH + x] = vga_entry(' ', color);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

static void fb_putc(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        return;
    }

    VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c, color);
    cursor_x++;

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
}

void fb_print(const char* str) {
    for (int i = 0; str[i]; i++) {
        fb_putc(str[i]);
    }
}
