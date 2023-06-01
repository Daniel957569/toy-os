#include "screen.h"
#include "../libc/string.h"
#include "ports.h"
#include "screen.h"

#define CURSOR_COMMAND_PORT 0x3d4
#define CURSOR_DATA_PORT 0x3d5

#define CURSOR_HIGH_BYTE_COMMAND 14
#define CURSOR_LOW_BYTE_COMMAND 15

#define SCREEN_POS_AS_CHAR(pos) ((screen_buffer[pos] & 0xFF))

static inline uint8_t vga_entry_color(vga_color fg, vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}

size_t screen_row;
size_t screen_column;
uint8_t screen_color;
uint16_t *screen_buffer;

static void move_cursor(uint16_t pos) {
    port_byte_out(CURSOR_COMMAND_PORT, 14);
    port_byte_out(CURSOR_DATA_PORT, (pos >> 8) & 0x00FF);
    port_byte_out(CURSOR_COMMAND_PORT, 15);
    port_byte_out(CURSOR_DATA_PORT, pos & 0x00FF);
}

void clear_screen(void) {
    screen_row = 0;
    screen_column = 0;
    screen_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    screen_buffer = (uint16_t *)0xB8000;
    move_cursor(0);

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            screen_buffer[index] = vga_entry(' ', screen_color);
        }
    }

    // BYTE_TO_BITS(screen_buffer[0]);
}

static inline size_t get_pos(size_t x, size_t y) { return y * VGA_WIDTH + x; }
static inline size_t current_screen_pos() {
    return screen_row * VGA_WIDTH + screen_column;
}

static void screen_setcolor(uint8_t color) { screen_color = color; }

void screen_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = get_pos(x, y);
    screen_buffer[index] = vga_entry(c, color);
}

static void copy_row(int source, int dest) {
    for (int i = 0; i < VGA_WIDTH; i++) {
        screen_buffer[dest * VGA_WIDTH + i] =
            screen_buffer[source * VGA_WIDTH + i];
    }
}

static void scroll() {
    for (int i = 0; i < VGA_HEIGHT; i++) {
        copy_row(i + 1, i);
    }

    // Clear the last row
    for (int i = 0; i < VGA_WIDTH; i++) {
        screen_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + i] =
            vga_entry(' ', screen_color);
    }
}

void screen_putchar(char c) {
    if (c == '\n') {
        screen_column = 0;
        if (++screen_row == VGA_HEIGHT) {
            scroll();
            --screen_row;
        }

        move_cursor(get_pos(0, screen_row));
        return;
    }

    screen_putentryat(c, screen_color, screen_column, screen_row);

    if (++screen_column == VGA_WIDTH) {
        screen_column = 0;
        if (++screen_row == VGA_HEIGHT) {
            scroll();
            --screen_row;
        }
    }
    move_cursor(get_pos(screen_column, screen_row));
}

void screen_backspace() {

    if (get_pos(screen_column, screen_row) == 0) {
        return;
    }

    if (screen_column == 0) {
        screen_row -= 1;
        screen_column = 79;
        size_t pos = get_pos(screen_column, screen_row);
        screen_buffer[pos] = screen_buffer[pos + 1];

        move_cursor(get_pos(screen_column, screen_row));
        return;
    }

    screen_column -= 1;
    size_t pos = get_pos(screen_column, screen_row);
    screen_buffer[pos] = screen_buffer[pos + 1];

    move_cursor(pos);
}

void screen_write(const char *data, size_t size) {
    for (size_t i = 0; i < size; i++)
        screen_putchar(data[i]);
}

void screen_print(char *data) { screen_write(data, strlen(data)); }

void screen_print_decimal(uint32_t n) {

    if (n == 0) {
        screen_putchar('0');
        return;
    }

    int32_t acc = n;

    char c[32];
    int i = 0;
    while (acc > 0) {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while (i >= 0) {
        c2[i--] = c[j++];
    }
    screen_print(c2);
}

void handle_ctrl_c(char *buffer) {
    // whatever
}

void screen_print_hex(uint32_t n) {
    int32_t tmp;

    screen_print("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0) {
            continue;
        }

        if (tmp >= 0xA) {
            noZeroes = 0;
            screen_putchar(tmp - 0xA + 'a');
        } else {
            noZeroes = 0;
            screen_putchar(tmp + '0');
        }
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) {
        screen_putchar(tmp - 0xA + 'a');
    } else {
        screen_putchar(tmp + '0');
    }
}

void handle_ctrl_w(char *buffer) {
    bool has_consume_char = false;
    while (1) {
        size_t pos = current_screen_pos();
        if (pos == 0 ||
            (SCREEN_POS_AS_CHAR(pos - 1)) == ' ' && has_consume_char)
            break;

        has_consume_char = true;
        screen_backspace();
        pop_char(buffer);
    }
}
