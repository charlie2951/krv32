#ifndef UART_H
#define UART_H

#include <stdint.h>

/**
 * UART Hardware Structure
 * The order of members must match the hardware memory map exactly.
 */
typedef struct {
    volatile uint32_t DATA;     // Offset 0x00
    volatile uint32_t CTRL;     // Offset 0x04
    volatile uint32_t STATUS;   // Offset 0x08
    uint32_t RESERVED[1];       // Offset 0x0C (Padding)
    volatile uint32_t RXDATA;   // Offset 0x10
    volatile uint32_t RXCTRL;   // Offset 0x14
    volatile uint32_t RXSTATUS; // Offset 0x18
} UART_TypeDef;

// Base Addresses
#define UART1_BASE 0x20000000
#define UART2_BASE 0x30000000

// Define handles for the UARTs
#define UART1 ((UART_TypeDef *) UART1_BASE)
#define UART2 ((UART_TypeDef *) UART2_BASE)

// Function Prototypes (Passing the UART handle)
void uart_send(UART_TypeDef *UARTx, uint8_t my_char);
uint8_t uart_receive(UART_TypeDef *UARTx);
void uart_sendline(UART_TypeDef *UARTx, const uint8_t *my_str);
void uart_init(UART_TypeDef *UARTx);
void uart_sendnumber(UART_TypeDef *UARTx,uint32_t val);
#endif
