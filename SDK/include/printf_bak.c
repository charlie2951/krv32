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
// Change n to unsigned long to avoid sign-extension issues during the call
static void print_number(unsigned long n, int base, int is_signed) {
    char buffer[33]; 
    int i = 0;
    const char *digits = "0123456789abcdef";

    unsigned long num;

    // Only handle negative signs if explicitly requested (for %d)
    if (is_signed && base == 10 && (long)n < 0) {
        uart_send(SYS_UART, (uint8_t)'-');
        num = (unsigned long)(-(long)n);
    } else {
        num = n;
    }

    if (num == 0) {
        uart_send(SYS_UART, (uint8_t)'0');
        return;
    }

    while (num > 0) {
        buffer[i++] = digits[num % base];
        num /= base;
    }

    while (i > 0) {
        uart_send(SYS_UART, (uint8_t)buffer[--i]);
    }
}
// --- The Main Printf Function ---
void printf(char *format, ...) {
    va_list args;
    va_start(args, format);

    for (char *p = format; *p != '\0'; p++) {
        if (*p == '%') {
            p++; 
            switch (*p) {
                case 's':
                    print_string(va_arg(args, char *));
                    break;
                case 'd':
                    // Cast to int, then unsigned long; flag is_signed = 1
                    print_number((unsigned long)va_arg(args, int), 10, 1);
                    break;
                case 'u':
                    // Treat as unsigned; flag is_signed = 0
                    print_number(va_arg(args, unsigned int), 10, 0);
                    break;
                case 'x':
                    print_string("0x");
                    print_number(va_arg(args, unsigned int), 16, 0);
                    break;
                case 'b':
                    print_string("0b");
                    print_number(va_arg(args, unsigned int), 2, 0);
                    break;
                case '%':
                    uart_send(SYS_UART, (uint8_t)'%');
                    break;
                default:
                    uart_send(SYS_UART, (uint8_t)'%');
                    uart_send(SYS_UART, (uint8_t)*p);
                    break;
            }
        } else {
            uart_send(SYS_UART, (uint8_t)*p);
        }
    }
    va_end(args);
}