#ifndef PRINTF_H
#define PRINTF_H

#include <stdarg.h>

/**
 * @brief Custom printf for bare-metal UART.
 * Supports: %s (string), %d (decimal), %x (hex), %b (binary), %% (percent)
 */
void printf(char *format, ...);

/**
 * @brief Hardware-specific character output. 
 * MUST be implemented in app.c or a separate UART driver.
 */
void UART_PutChar(char c);

#endif // PRINTF_H