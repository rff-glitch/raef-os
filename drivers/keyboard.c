#include "keyboard.h"

#define KEYBOARD_DATA   0x60
#define KEYBOARD_STATUS 0x64

static char scancode_map[256] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' '
};

static int head = 0, tail = 0;

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void keyboard_init(void) {
    head = tail = 0;
}

char keyboard_get_char(void) {
    if (!(inb(KEYBOARD_STATUS) & 1))
        return 0;

    unsigned char sc = inb(KEYBOARD_DATA);
    if (sc & 0x80) return 0;

    char c = scancode_map[sc];
    return c;
}

void keyboard_flush(void) {
    head = tail = 0;
}