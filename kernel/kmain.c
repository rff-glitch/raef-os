#include "../drivers/framebuffer.h"

#define KEYBOARD_DATA 0x60
#define KEYBOARD_STATUS 0x64

char input_buffer[128];
int buffer_index = 0;


// Minimal strcmp
int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char*)a - *(unsigned char*)b;
}

// Simple scancode map
char scancode_map[256] = {
    0, 27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0, '*',0,' '
};

char inb(int port) {
    char ret;
    __asm__ volatile ("inb %1,%0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void Flex() {
        fb_print("\n"
            " _______________________\n"
            "\n"
            " ______                __ \n"
            " | ___ \\              / _| \n"
            " | |_/ /  __ _   ___ | |_ \n"
            " |    /  / _` | / _ \\|  _|\n"
            " | |\\ \\ | (_| ||  __/| |  \n"
            " \\_| \\_| \\__,_| \\___||_|  \n"
            "                  xby raef\n"
            "\n"
        );
}

void keyboard_handler() {
    if (!(inb(KEYBOARD_STATUS) & 1)) return; 

    char scancode = inb(KEYBOARD_DATA);
    if (scancode & 0x80) return; 

    char c = scancode_map[(int)scancode];
    if (c == '\b' && buffer_index > 0) {
        buffer_index--;
        fb_print("\b \b"); 
    } else if (c == '\n') {
        fb_print("\n");
        input_buffer[buffer_index] = 0;
        buffer_index = 0;

        if (strcmp(input_buffer, "help") == 0)
            fb_print("Hi there ,\n this is Raef-OS programed from scratch by raef or rff glitch \n Commands: help, clear , flex \n");
        else if (strcmp(input_buffer, "clear") == 0)
            fb_clear();
        else if (strcmp(input_buffer, "flex") == 0)
            Flex();
        else
            fb_print("Unknown command\n");

        fb_print("> ");
    } else if (c) {
        input_buffer[buffer_index++] = c;
        char s[2] = {c,0};
        fb_print(s);
    }
}

void kmain(void) {
    fb_clear();
    fb_print("Raef OS by rfff-glitch\n> ");

    while (1) {
        keyboard_handler();
    }
}

