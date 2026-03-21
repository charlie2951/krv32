#include "printf.h"
#include "uart.h"
#define SYS_UART UART1

// --- Helper: Print Strings ---
static void print_string(char *s) {
    if (!s) s = "(null)";
    while (*s) {
        uart_send(SYS_UART,(uint8_t)*s++);
    }
}
/*
// --- Helper: Print Numbers (Base 2, 10, 16) ---
static void print_number(long n, int base) {
    char buffer[33];
    int i = 0;
    char *digits = "0123456789abcdef";

    // Handle negative decimals
    if (base == 10 && n < 0) {
        uart_send(SYS_UART,(uint8_t)'-');
        n = -n;
    }

    unsigned long num = (unsigned long)n;

    if (num == 0) {
        uart_send(SYS_UART,(uint8_t)'0');
        return;
    }

    while (num > 0) {
        buffer[i++] = digits[num % base];
        num /= base;
    }

    while (i > 0) {
      i= --i;
        uart_send(SYS_UART,(uint8_t)buffer[--i]);
    }
}
*/
static void print_number(long n, int base) {
    char buffer[33]; // 32 bits + null terminator
    int i = 0;
    const char *digits = "0123456789abcdef";

    // 1. Handle Negative Decimals
    unsigned long num;
    if (base == 10 && n < 0) {
        uart_send(SYS_UART,(uint8_t)'-');
        num = (unsigned long)(-n);
    } else {
        num = (unsigned long)n;
    }

    // 2. Handle Zero explicitly
    if (num == 0) {
        uart_send(SYS_UART,(uint8_t)'0');
        return;
    }

    // 3. Conversion Loop
    // Ensure we use 'num' everywhere here, not 'n'
    while (num > 0) {
        buffer[i++] = digits[num % base];
        num /= base;
    }

    // 4. Print in Reverse
    while (i > 0) {
        uart_send(SYS_UART,(uint8_t)buffer[--i]);
    }
}
// --- The Main Printf Function ---
void printf(char *format, ...) {
    va_list args;
    va_start(args, format);

    for (char *p = format; *p != '\0'; p++) {
        if (*p == '%') {
            p++; // Skip '%'
            switch (*p) {
                case 's':
                    print_string(va_arg(args, char *));
                    break;
                case 'd':
                    print_number(va_arg(args, int), 10);
                    break;
                case 'x':
                    print_string("0x");
                    print_number(va_arg(args, unsigned int), 16);
                    break;
                case 'b':
                    print_string("0b");
                    print_number(va_arg(args, unsigned int), 2);
                    break;
                case '%':
                    uart_send(SYS_UART,(uint8_t)'%');
                    break;
                default:
                    uart_send(SYS_UART,(uint8_t)'%');
                    uart_send(SYS_UART,(uint8_t)*p);
                    break;
            }
        } else {
            uart_send(SYS_UART,(uint8_t)*p);
        }
    }
    va_end(args);
}
