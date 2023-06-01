#include "keyboard.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "ports.h"
#include "screen.h"

#define get_char(ch1, ch2) (caps_lock ? ch1 : ch2)
#define char_uppercase(ch) (char)ch - 32

static char key_buffer[256];

bool caps_lock = false;
bool ctrl_pressed = false;

void print_letter(uint8_t scancode);

static void handle_key_press(char c) {
    append(key_buffer, c);
    screen_putchar(c);
}

static void handle_backspace() {
    screen_backspace();
    pop_char(key_buffer);
}

static void handle_char_press(char c) {
    if (ctrl_pressed) {
        switch (c) {
        case 'w':
            handle_ctrl_w(key_buffer);
        case 'c':
            handle_ctrl_c(key_buffer);
        defualt:
            return;
        }
    }

    char ch = get_char(char_uppercase(c), c);
    append(key_buffer, ch);
    screen_putchar(ch);
}

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
        // screen_print("ERROR");
        break;
    case 0x1:
        // screen_print("ESC");
        break;
    case 0x2:
        handle_key_press('1');
        break;
    case 0x3:
        handle_key_press('2');
        break;
    case 0x4:
        handle_key_press('3');
        break;
    case 0x5:
        handle_key_press('4');
        break;
    case 0x6:
        handle_key_press('5');
        break;
    case 0x7:
        handle_key_press('6');
        break;
    case 0x8:
        handle_key_press('7');
        break;
    case 0x9:
        handle_key_press('8');
        break;
    case 0x0A:
        handle_key_press('9');
        break;
    case 0x0B:
        handle_key_press('0');
        break;
    case 0x0C:
        handle_key_press('-');
        break;
    case 0x0D:
        handle_key_press('+');
        break;
    case 0x0E:
        handle_backspace();
        // handle_key_press('Backspace');
        break;
    case 0x0F:
        // handle_key_press('Tab');
        break;
    case 0x10:
        handle_char_press('q');
        break;
    case 0x11:
        handle_char_press('w');
        break;
    case 0x12:
        handle_char_press('e');
        break;
    case 0x13:
        handle_char_press('r');
        break;
    case 0x14:
        handle_char_press('t');
        break;
    case 0x15:
        handle_char_press('y');
        break;
    case 0x16:
        handle_char_press('u');
        break;
    case 0x17:
        handle_char_press('i');
        break;
    case 0x18:
        handle_char_press('o');
        break;
    case 0x19:
        handle_char_press('p');
        break;
    case 0x1A:
        handle_key_press('[');
        break;
    case 0x1B:
        handle_key_press(']');
        break;
    case 0x1C:
        screen_print(key_buffer);
        clear_string(key_buffer);
        screen_print("\n$");

        // handle enter key.
        break;
    case 0x1D:
        ctrl_pressed = true;
        // handle_key_press('LCtrl');
        break;
    case 0x1E:
        handle_char_press('a');
        break;
    case 0x1F:
        handle_char_press('s');
        break;
    case 0x20:
        handle_char_press('d');
        break;
    case 0x21:
        handle_char_press('f');
        break;
    case 0x22:
        handle_char_press('g');
        break;
    case 0x23:
        handle_char_press('h');
        break;
    case 0x24:
        handle_char_press('j');
        break;
    case 0x25:
        handle_char_press('k');
        break;
    case 0x26:
        handle_char_press('l');
        break;
    case 0x27:
        handle_key_press(';');
        break;
    case 0x28:
        handle_key_press('\'');
        break;
    case 0x29:
        handle_key_press('`');
        break;
    case 0x2A:
        // handle_key_press('LShift');
        break;
    case 0x2B:
        handle_key_press('\\');
        break;
    case 0x2C:
        handle_char_press('z');
        break;
    case 0x2D:
        handle_char_press('x');
        break;
    case 0x2E:
        handle_char_press('c');
        break;
    case 0x2F:
        handle_char_press('v');
        break;
    case 0x30:
        handle_char_press('b');
        break;
    case 0x31:
        handle_char_press('n');
        break;
    case 0x32:
        handle_char_press('m');
        break;
    case 0x33:
        handle_key_press(',');
        break;
    case 0x34:
        handle_key_press('.');
        break;
    case 0x35:
        handle_key_press('/');
        break;
    case 0x36:
        // handle_key_press('Rshift');
        break;
    case 0x37:
        // handle_key_press('Keypad *');
        break;
    case 0x38:
        // handle_key_press('LAlt');
        break;
    case 0x39:
        handle_key_press(' ');
        // screen_print("Spc");
        break;
    case 0x3A:
        caps_lock = !caps_lock;
        break;
    case 0x9D:
        ctrl_pressed = false;
        break;
    default:
        break;
    }
}
