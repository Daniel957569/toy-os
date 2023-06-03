
#ifndef DEBUG_H
#define DEBUG_H


#include <stdbool.h>

#include "printf.h"

#include "../display/vga.h"
#include "../display/screen.h"

// #include "../boot/multiboot.h"


/** Panicking macro. */
#define panic(fmt, args...) do {                                               \
                                asm volatile ( "cli" );                        \
                                cprintf(VGA_COLOR_MAGENTA, "PANIC: " fmt "\n", \
                                        ##args);                               \
                                while (1)                                      \
                                  asm volatile ( "hlt" );                      \
                                __builtin_unreachable();                       \
                            } while (0)


/** Assertion macro. */
#define assert(condition)   do {                                              \
                                if (!(condition)) {                           \
                                    printf_to_hold_lock = false;              \
                                    panic("assertion failed @ function '%s'," \
                                          " file '%s': line %d",              \
                                          __FUNCTION__, __FILE__, __LINE__);  \
                                }                                             \
                            } while (0)


/** Error prompting macro. */
#define error(fmt, args...) do {                                           \
                                cprintf(VGA_COLOR_RED, "ERROR: " fmt "\n", \
                                        ##args);                           \
                                panic("error occurred @ function '%s',"    \
                                      " file '%s': line %d",               \
                                      __FUNCTION__, __FILE__, __LINE__);   \
                            } while (0)


/** Warning prompting macro. */
#define warn(fmt, args...)  do {                                              \
                                cprintf(VGA_COLOR_MAGENTA, "WARN: " fmt "\n", \
                                        ##args);                              \
                            } while (0)


/** Info prompting macro. */
#define info(fmt, args...)  do {                                           \
                                cprintf(VGA_COLOR_CYAN, "INFO: " fmt "\n", \
                                        ##args);                           \
                            } while (0)


#endif
