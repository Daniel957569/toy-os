#ifndef SCREEN_H
#define SCREEN_H

#include "../libc/common.h"
#include "vga.h"

extern const vga_color_t TERMINAL_DEFAULT_COLOR_BG;
extern const vga_color_t TERMINAL_DEFAULT_COLOR_FG;


void terminal_init();

void terminal_write(const char *data, size_t size);
void terminal_write_color(const char *data, size_t size, vga_color_t fg);

void terminal_clear();

#endif
