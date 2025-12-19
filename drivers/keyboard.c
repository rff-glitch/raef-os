#include "keyboard.h"
#include "../lib/stdint.h"
#include "../kernel/shell.h"

#define KEYBOARD_DATA   0x60
#define KEYBOARD_STATUS 0x64

static char scancode_map[256] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'',0,0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' '
};

static char scancode_shift_map[256] = {
    0,27,'!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,
    'A','S','D','F','G','H','J','K','L',':','"','~',0,'|',
    'Z','X','C','V','B','N','M','<','>','?',0,'*',0,' '
};

static int shift_pressed = 0;
static int caps_lock = 0;

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void keyboard_init(void) {
    shift_pressed = 0;
    caps_lock = 0;
}

char keyboard_get_char(void) {
    if (!(inb(KEYBOARD_STATUS) & 1))
        return 0;

    unsigned char sc = inb(KEYBOARD_DATA);

    if (sc & 0x80) {
        sc &= 0x7F; 
        if (sc == 0x2A || sc == 0x36) shift_pressed = 0;
        return 0;
    }

   
    if (sc == 0x2A || sc == 0x36) {
        shift_pressed = 1;
        return 0;
    }

    
    if (sc == 0x3A) {
        caps_lock = !caps_lock;
        return 0;
    }

    char c;
    if (shift_pressed)
        c = scancode_shift_map[sc];
    else
        c = scancode_map[sc];

   
    if (caps_lock && c >= 'a' && c <= 'z') 
        c -= 32; 

    return c;
}

void keyboard_flush(void) {

    shift_pressed = 0;
    caps_lock = 0;
}