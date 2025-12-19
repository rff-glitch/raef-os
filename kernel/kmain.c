#include "../drivers/framebuffer.h"
#include "../drivers/keyboard.h"
#include "../lib/stddef.h"
#include "../lib/stdint.h"
#include "shell.h"


unsigned long uptime_ticks = 0;

void kmain(void) {
    fb_clear();
    fb_print("Raef OS\n");
    keyboard_init();
    shell_init();
    while (1) {
        char c = keyboard_get_char();
        if (c) {
            shell_process_char(c);
        }
        uptime_ticks++;
    }
}
