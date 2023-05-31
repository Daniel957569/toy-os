#include "screen.h"
#include "../kernel/util.h"
#include "ports.h"
#include "screen.h"

#define CURSOR_COMMAND_PORT 0x3d4
#define CURSOR_DATA_PORT 0x3d5

#define CURSOR_HIGH_BYTE_COMMAND 14
#define CURSOR_LOW_BYTE_COMMAND 15

#define SCREEN_POS_AS_CHAR(pos) ((screen_buffer[pos] & 0xFF))

static void BYTE_TO_BITS(int byte) {
    screen_print("yooo the byte: ");
    (byte & 0x8000) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x4000) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x2000) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x1000) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x0800) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x0400) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x0200) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x0100) ? screen_putchar('1') : screen_putchar('0');
    screen_putchar(' ');
    (byte & 0x0080) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x0040) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x0020) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x0010) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x0008) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x0004) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x0002) ? screen_putchar('1') : screen_putchar('0');
    (byte & 0x0001) ? screen_putchar('1') : screen_putchar('0');
    screen_putchar('\n');

    screen_print("upper 8 bits = ");
    char str[100];
    int byte1 = byte;
    int_to_ascii((byte1 >> 8) & 0xFF, str);
    screen_print(str);
    screen_putchar('\n');

    screen_print("lower 8 bits = ");
    char str2[100];
    int byte2 = byte;
    int_to_ascii(byte2 & 0xFF, str2);
    screen_print(str2);
    screen_putchar('\n');
}

static inline uint8_t vga_entry_color(vga_color fg, vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len])
        len++;

    return len;
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

void screen_print(const char *data) { screen_write(data, strlen(data)); }

void handle_ctrl_c() {
    // whatever
}

void handle_ctrl_w() {
    bool has_consume_char = false;
    while (1) {
        size_t pos = current_screen_pos();
        if (pos == 0 ||
            (SCREEN_POS_AS_CHAR(pos - 1)) == ' ' && has_consume_char)
            break;

        has_consume_char = true;
        screen_backspace();
    }
}
