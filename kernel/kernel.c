#include "../drivers/screen.h"


void kernel_main(void) {
    /* Initialize terminal interfacem */
    terminal_initialize();
    terminal_writestring("Hello World\n sfdsf");
}
