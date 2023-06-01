#ifndef SCREEN_H
#define SCREEN_H

#include "../common.h"

#define VIDEO_ADDRESS 0xb8000

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color;

/* Public kernel API */
void clear_screen();
void screen_putchar(char c);
void screen_backspace();
void screen_print(char *message);
void screen_print_decimal(uint32_t n);
void screen_print_hex(uint32_t n);

void handle_ctrl_w(char* buffer);
void handle_ctrl_c(char* buffer);

#endif
