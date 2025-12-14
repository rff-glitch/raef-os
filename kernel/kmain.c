#include "../drivers/framebuffer.h"

#define KEYBOARD_DATA   0x60
#define KEYBOARD_STATUS 0x64

static char input_buffer[128];
static int buffer_index = 0;

static unsigned long uptime_ticks = 0;

/* minimal strcmp */
int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char*)a - *(unsigned char*)b;
}

/* s ASCII (US layout, minimal) */
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

void help(void){
    fb_print(
        "Commands:\n"
        "  help      - show this message\n"
        "  clear     - clear screen\n"
        "  flex      - ascii art\n"
        "  uptime    - show uptime\n"
        "  reboot    - reboot system\n"
        "  echo <txt>- print text\n"
    );
}


static void flex(void) {
    fb_print(
        "\n"
        " ________            _____ \n"
        " | ___ \\           |  ___|\n"
        " | |_/ /__ _  ___   | |__  \n"
        " |    // _` |/ _ \\ |  __| \n"
        " | |\\ \\ (_| |  __/| |    \n"
        " \\_| \\_\\__,_|\\__|_|    \n"
        "           by rfff-glitch \n\n"
    );
}

static void echo(const char* input) {

    if (!input[5]) {
        fb_print("\n");
        return;
    }

    fb_print(input + 5);
    fb_print("\n");
}

void reboot(void) {
    fb_print("Rebooting...\n");
    __asm__ volatile (
        "cli\n"
        "mov $0xFE, %al\n"
        "out %al, $0x64\n"
    );
    while (1) { }
}

static void uptime(void) {
    unsigned long t = uptime_ticks / 100000;
    char buf[32];
    int i = 0;

    if (t == 0) {
        buf[i++] = '0';
    } else {
        char tmp[32];
        int j = 0;

        while (t > 0) {
            tmp[j++] = '0' + (t % 10);
            t /= 10;
        }

        while (j > 0) {
            buf[i++] = tmp[--j];
        }
    }

    buf[i++] = '\n';
    buf[i] = 0;

    fb_print("Uptime: ");
    fb_print(buf);
}


static void handle_command(void) {
    if (strcmp(input_buffer, "help") == 0) {
        help();
    } else if (strcmp(input_buffer, "clear") == 0) {
        fb_clear();  
    }else if (
        input_buffer[0] == 'e' &&
        input_buffer[1] == 'c' &&
        input_buffer[2] == 'h' &&
        input_buffer[3] == 'o' &&
        input_buffer[4] == ' ') {
        echo(input_buffer);  
    }else if (strcmp(input_buffer, "flex") == 0) {
        flex();
    } else if (strcmp(input_buffer, "uptime") == 0) {
        uptime();
    }else if (strcmp(input_buffer, "reboot") == 0) {
        reboot();
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
    fb_print("Raef OS \n");
    flex();
    help();
    fb_print("> ");

    while (1) {
        keyboard_poll();
        uptime_ticks++;
    }
}