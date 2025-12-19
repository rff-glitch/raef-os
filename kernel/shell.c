#include "../drivers/framebuffer.h"
#include "../drivers/keyboard.h"
#include "shell.h"
#include <stdint.h>
#include <stddef.h>




static unsigned int kstrlen(const char* s) {
    unsigned int i = 0;
    while (s[i]) i++;
    return i;
}

static int kstrncmp(const char* a, const char* b, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        if (a[i] != b[i] || !a[i] || !b[i]) {
            return (unsigned char)a[i] - (unsigned char)b[i];
        }
    }
    return 0;
}


#define INPUT_BUFFER_SIZE 128

static char input_buffer[INPUT_BUFFER_SIZE];
static unsigned int buffer_index = 0;

extern unsigned long uptime_ticks;


static void help(const char* args);
static void clear(const char* args);
static void echo(const char* args);
static void flex(const char* args);
static void uptime(const char* args);
static void reboot(const char* args);
static void shutdown(const char* args);

typedef void (*command_fn)(const char*);

typedef struct {
    const char* name;
    command_fn func;
} command_t;

static command_t commands[] = {
    {"help",     help},
    {"clear",    clear},
    {"echo",     echo},
    {"flex",     flex},
    {"uptime",   uptime},
    {"reboot",   reboot},
    {"shutdown", shutdown},
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))


static void handle_command(const char* line) {
    for (unsigned int i = 0; i < COMMAND_COUNT; i++) {
        unsigned int name_len = kstrlen(commands[i].name);

        if (kstrncmp(line, commands[i].name, name_len) == 0 &&
            (line[name_len] == 0 || line[name_len] == ' ')) {

            const char* args =
                (line[name_len] == ' ') ? line + name_len + 1 : 0;

            commands[i].func(args);
            return;
        }
    }

    if (line[0]) {
        fb_print("Unknown command\n");
    }
}

void shell_process_char(char c) {
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
        handle_command(input_buffer);
        buffer_index = 0;
        fb_print("> ");
        return;
    }

    if (buffer_index < INPUT_BUFFER_SIZE - 1) {
        input_buffer[buffer_index++] = c;
        char s[2] = { c, 0 };
        fb_print(s);
    }
}


static void help(const char* args) {
    (void)args;
    fb_print(
        "Commands:\n"
        "  help\n"
        "  clear\n"
        "  echo <text>\n"
        "  flex\n"
        "  uptime\n"
        "  reboot\n"
        "  shutdown\n"
    );
}

static void clear(const char* args) {
    (void)args;
    fb_clear();
}

static void echo(const char* args) {
    if (!args || !*args) {
        fb_print("\n");
        return;
    }
    fb_print(args);
    fb_print("\n");
}

static void flex(const char* args) {
    (void)args;
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

static void uptime(const char* args) {
    (void)args;

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

static void reboot(const char* args) {
    (void)args;
    fb_print("Rebooting...\n");
    __asm__ volatile (
        "cli\n"
        "mov $0xFE, %al\n"
        "out %al, $0x64\n"
    );
    while (1) {
        __asm__ volatile ("hlt");
    }
}

static void shutdown(const char* args) {
    (void)args;
    fb_print("Attempting system power-off...\n");

    __asm__ volatile (
        "cli\n"
        "mov $0x5307, %%ax\n"
        "mov $0x0001, %%bx\n"
        "mov $0x0003, %%cx\n"
        "int $0x15\n"
        :
        :
        : "ax", "bx", "cx"
    );

    while (1) {
        __asm__ volatile ("hlt");
    }
}

void shell_init(void) {
    flex(0);
    help(0);
    fb_print("> ");
}