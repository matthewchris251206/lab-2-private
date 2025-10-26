#include "basic.h"

#include <stddef.h>

void terminate(void) {
    volatile uint8_t *halt = (uint8_t *)HALT_BASE;
    *halt                  = 1;
    for (;;) {
        // infinite loop
    }
}

void platform_outb(char c) {
    volatile char *text_buf = (char *)TEXT_BUFFER_BASE;
    *text_buf               = c;
}

int start(void) {
    // initialize .bss section
    extern char _sbss[];
    extern char _ebss[];
    for (unsigned long i = (unsigned long)_sbss; i < (unsigned long)_ebss; i++) {
        *(uint8_t *)i = 0; // zero initialize byte-by-byte
    }

    // copy .data section from ROM to RAM
    extern char _sidata[];
    extern char _sdata[];
    extern char _edata[];
    size_t data_copy_size = (size_t)(_edata - _sdata);
    uint8_t *src_ptr      = (uint8_t *)_sidata;
    uint8_t *dst_ptr      = (uint8_t *)_sdata;
    for (size_t i = 0; i < data_copy_size; i++) {
        *(dst_ptr++) = *(src_ptr++);
    }

    extern int main(void);
    return main();
}
