#include "../drivers/framebuffer.h"

#define KEYBOARD_DATA   0x60
#define KEYBOARD_STATUS 0x64

static char input_buffer[128];
static int buffer_index = 0;

/* minimal strcmp */
int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char*)a - *(unsigned char*)b;
}

/* scancode → ASCII (US layout, minimal) */
static char scancode_map[256] = {
    0, 27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' '
};

static unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static void flex(void) {
    fb_print(
        "\n"
        " ________            _____ \n"
        " | ___ \\           |  ___|\n"
        " | |_/ /__ _  ___   | |__  \n"
        " |    // _` |/ _ \\ |  __| \n"
        " | |\\ \\ (_| |  __/| | \n"
        " \\_| \\_\\__,_|\\__|_| \n"
        "           by rfff-glitch\n\n"
    );
}

static void handle_command(void) {
    if (strcmp(input_buffer, "help") == 0) {
        fb_print(
            "Commands:\n"
            "  help  - show this message\n"
            "  clear - clear screen\n"
            "  flex  - ascii art\n"
        );
    } else if (strcmp(input_buffer, "clear") == 0) {
        fb_clear();
    } else if (strcmp(input_buffer, "flex") == 0) {
        flex();
    } else if (input_buffer[0]) {
        fb_print("Unknown command\n");
    }
}

static void keyboard_poll(void) {
    if (!(inb(KEYBOARD_STATUS) & 1))
        return;

    unsigned char scancode = inb(KEYBOARD_DATA);

    if (scancode & 0x80)
        return;

    char c = scancode_map[scancode];
    if (!c)
        return;

    if (c == '\b') {
        if (buffer_index > 0) {
            buffer_index--;
            fb_print("\b");
        }
        return;
    }

    if (c == '\n') {
        fb_print("\n");
        input_buffer[buffer_index] = 0;
        handle_command();
        buffer_index = 0;
        fb_print("> ");
        return;
    }

    if (buffer_index < 127) {
        input_buffer[buffer_index++] = c;
        char s[2] = { c, 0 };
        fb_print(s);
    }
}

void kmain(void) {
    fb_clear();
    fb_print("Raef OS by rfff-glitch\n");
    fb_print("> ");

    while (1) {
        keyboard_poll();
    }
}