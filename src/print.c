#include "print.h"

#include "basic.h"

#include <stdarg.h>

void my_putc(char c) {
    platform_outb(c);
}

void my_puts(char *s) {
    while (*s != '\0') {
        my_putc(*(s++));
    }
}

void my_putint(int numb) {
    if (numb < 0) {
        my_putc('-');
        numb = -numb; // convert to positive number
    }

    if (numb / 10) {
        my_putint(numb / 10);
    }
    my_putc((numb % 10) + '0');
}

void my_printf(char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
            case 'c': { // char
                char c = va_arg(args, int);
                my_putc(c);
                break;
            }
            case 'd': { // int
                int numb = va_arg(args, int);
                my_putint(numb);
                break;
            }
            case 's': { // string
                char *s = va_arg(args, char *);
                my_puts(s);
                break;
            }
            default: break;
            }
        } else {
            my_putc(*format);
        }
        format++;
    }

    va_end(args);
}
