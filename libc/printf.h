#ifndef PRINTF_H
#define PRINTF_H

#include "../display/vga.h"

void printf(const char *fmt, ...);
void cprintf(vga_color_t fg, const char *fmt, ...);

void snprintf(char *buf, size_t count, const char *fmt, ...);

#endif
