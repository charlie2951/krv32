#include "printf.h"
#include "uart.h"
#include <stdarg.h>

#define SYS_UART UART1

/**
 * Helper: Send a string over UART
 */
static void print_string(char *s) {
    if (!s) s = "(null)";
    while (*s) {
        uart_send(SYS_UART, (uint8_t)*s++);
    }
}

/**
 * Helper: Convert a number to a string and send over UART
 * Supports base 2, 10, and 16 with custom width and zero-padding.
 */
static void print_number(unsigned long n, int base, int is_signed, int width, int pad_zero) {
    char buffer[33]; 
    int i = 0;
    const char *digits = "0123456789abcdef";
    unsigned long num;

    // Handle signed decimals
    if (is_signed && base == 10 && (long)n < 0) {
        uart_send(SYS_UART, (uint8_t)'-');
        num = (unsigned long)(-(long)n);
    } else {
        num = n;
    }

    // Convert number to string in reverse order
    if (num == 0) {
        buffer[i++] = '0';
    } else {
        while (num > 0) {
            buffer[i++] = digits[num % base];
            num /= base;
        }
    }

    // Handle Padding: Add zeros or spaces to reach the target width
    // 'i' is the current digit count
    while (i < width) {
        uart_send(SYS_UART, (uint8_t)(pad_zero ? '0' : ' '));
        width--;
    }

    // Print the digits from the buffer (correct order)
    while (i > 0) {
        uart_send(SYS_UART, (uint8_t)buffer[--i]);
    }
}

/**
 * Main Printf Function
 * Supports: %s, %d, %u, %x, %b, %%
 * Supports format modifiers like %08x or %0.2d
 */
void printf(char *format, ...) {
    va_list args;
    va_start(args, format);

    for (char *p = format; *p != '\0'; p++) {
        if (*p == '%') {
            p++; // Move past '%'

            int width = 0;
            int pad_zero = 0;

            // 1. Detect padding style (e.g., %0...)
            if (*p == '0') {
                pad_zero = 1;
                p++;
            }

            // 2. Detect precision/dot style (e.g., %0.2d)
            if (*p == '.') {
                pad_zero = 1; // Precision implies zero padding for numbers
                p++;
            }

            // 3. Parse width digits (e.g., the '8' in %08x)
            while (*p >= '0' && *p <= '9') {
                width = width * 10 + (*p - '0');
                p++;
            }

            // 4. Handle type specifier
            switch (*p) {
                case 's':
                    print_string(va_arg(args, char *));
                    break;

                case 'd':
                    print_number((unsigned long)va_arg(args, int), 10, 1, width, pad_zero);
                    break;

                case 'u':
                    print_number(va_arg(args, unsigned int), 10, 0, width, pad_zero);
                    break;

                case 'x':
                    // Print hex prefix and pad the actual value
                    print_string("0x");
                    print_number(va_arg(args, unsigned int), 16, 0, width, pad_zero);
                    break;

                case 'b':
                    print_string("0b");
                    print_number(va_arg(args, unsigned int), 2, 0, width, pad_zero);
                    break;

                case '%':
                    uart_send(SYS_UART, (uint8_t)'%');
                    break;

                default:
                    // If unknown, print the '%' and the character as-is
                    uart_send(SYS_UART, (uint8_t)'%');
                    uart_send(SYS_UART, (uint8_t)*p);
                    break;
            }
        } else {
            // Regular character
            uart_send(SYS_UART, (uint8_t)*p);
        }
    }
    va_end(args);
}