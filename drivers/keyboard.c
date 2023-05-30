#include "keyboard.h"
#include "../cpu/isr.h"
#include "../kernel/util.h"
#include "ports.h"
#include "screen.h"

#define print_char(ch1, ch2) screen_print(shift ? ch1 : ch2)

bool shift = false;

void print_letter(uint8_t scancode);

static void keyboard_callback(registers_t regs) {
    uint8_t scancode = port_byte_in(0x60);

    char *sc_ascii;
    int_to_ascii(scancode, sc_ascii);
    print_letter(scancode);
}

void init_keyboard() { register_interrupt_handler(IRQ1, keyboard_callback); }

void print_letter(uint8_t scancode) {
    switch (scancode) {
    case 0x0:
        screen_print("ERROR");
        break;
    case 0x1:
        // screen_print("ESC");
        break;
    case 0x2:
        screen_print("1");
        break;
    case 0x3:
        screen_print("2");
        break;
    case 0x4:
        screen_print("3");
        break;
    case 0x5:
        screen_print("4");
        break;
    case 0x6:
        screen_print("5");
        break;
    case 0x7:
        screen_print("6");
        break;
    case 0x8:
        screen_print("7");
        break;
    case 0x9:
        screen_print("8");
        break;
    case 0x0A:
        screen_print("9");
        break;
    case 0x0B:
        screen_print("0");
        break;
    case 0x0C:
        screen_print("-");
        break;
    case 0x0D:
        screen_print("+");
        break;
    case 0x0E:
        // screen_print("Backspace");
        break;
    case 0x0F:
        screen_print("Tab");
        break;
    case 0x10:
        print_char("Q", "q");
        break;
    case 0x11:
        print_char("W", "w");
        break;
    case 0x12:
        print_char("E", "w");
        break;
    case 0x13:
        print_char("R", "r");
        break;
    case 0x14:
        print_char("T", "t");
        break;
    case 0x15:
        print_char("Y", "y");
        break;
    case 0x16:
        print_char("U", "u");
        break;
    case 0x17:
        print_char("I", "i");
        break;
    case 0x18:
        print_char("O", "o");
        break;
    case 0x19:
        print_char("P", "p");
        break;
    case 0x1A:
        screen_print("[");
        break;
    case 0x1B:
        screen_print("]");
        break;
    case 0x1C:
        // screen_print("ENTER");
        break;
    case 0x1D:
        // screen_print("LCtrl");
        break;
    case 0x1E:
        print_char("A", "a");
        break;
    case 0x1F:
        print_char("S", "s");
        break;
    case 0x20:
        print_char("D", "d");
        break;
    case 0x21:
        print_char("F", "f");
        break;
    case 0x22:
        print_char("G", "g");
        break;
    case 0x23:
        print_char("H", "h");
        break;
    case 0x24:
        print_char("J", "j");
        break;
    case 0x25:
        print_char("K", "k");
        break;
    case 0x26:
        print_char("L", "l");
        break;
    case 0x27:
        screen_print(";");
        break;
    case 0x28:
        screen_print("'");
        break;
    case 0x29:
        screen_print("`");
        break;
    case 0x2A:
        // screen_print("LShift");
        break;
    case 0x2B:
        screen_print("\\");
        break;
    case 0x2C:
        print_char("Z", "z");
        break;
    case 0x2D:
        print_char("X", "x");
        break;
    case 0x2E:
        print_char("C", "c");
        break;
    case 0x2F:
        print_char("V", "v");
        break;
    case 0x30:
        print_char("B", "b");
        break;
    case 0x31:
        print_char("N", "n");
        break;
    case 0x32:
        print_char("M", "m");
        break;
    case 0x33:
        screen_print(",");
        break;
    case 0x34:
        screen_print(".");
        break;
    case 0x35:
        screen_print("/");
        break;
    case 0x36:
        // screen_print("Rshift");
        break;
    case 0x37:
        // screen_print("Keypad *");
        break;
    case 0x38:
        // screen_print("LAlt");
        break;
    case 0x39:
        // screen_print("Spc");
        break;
    case 0x3A:
        shift = !shift;
        break;
    default:
        break;
    }
}
