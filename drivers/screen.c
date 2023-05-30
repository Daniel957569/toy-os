#include "screen.h"
#include "port.h"

#define CURSOR_COMMAND_PORT 0x3d4
#define CURSOR_DATA_PORT 0x3d5

#define CURSOR_HIGH_BYTE_COMMAND 14
#define CURSOR_LOW_BYTE_COMMAND 15

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

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void move_cursor(uint8_t pos) {
    port_byte_out(CURSOR_COMMAND_PORT, 14);
    port_byte_out(CURSOR_DATA_PORT, (pos >> 8) & 0x00FF);
    port_byte_out(CURSOR_COMMAND_PORT, 15);
    port_byte_out(CURSOR_DATA_PORT, pos & 0x00FF);
}

void terminal_initialize(void) {
    // __asm__("out %%al, %%dx" : : "a"(14), "d"(0x3d4));
    // __asm__("out %%al, %%dx" : : "a"((40 >> 8) & 0x00FF), "d"(0x3d5));
    // __asm__("out %%al, %%dx" : : "a"(15), "d"(0x3d4));
    // __asm__("out %%al, %%dx" : : "a"(40 & 0x00FF), "d"(0x3d5));
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t *)0xB8000;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row += 1;
        move_cursor(terminal_row * VGA_WIDTH);
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
    move_cursor(terminal_column + terminal_row * 80);
}

void terminal_write(const char *data, size_t size) {
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {
    terminal_write(data, strlen(data));
}
