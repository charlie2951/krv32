#include "uart.h"
#include "delay.h"

void uart_send(UART_TypeDef *UARTx, uint8_t my_char) {
    // Wait until the specific UART's status is ready
    while(UARTx->STATUS == 0); 
    
    UARTx->DATA = my_char;
    UARTx->CTRL = 1;
    UARTx->CTRL = 0;
}

uint32_t uart_receive(UART_TypeDef *UARTx) {
    UARTx->RXCTRL = 1; // Enable
    UARTx->RXCTRL = 0; // Pulse
    while(UARTx->RXSTATUS == 0);
    return UARTx->RXDATA;
}

void uart_sendline(UART_TypeDef *UARTx, uint8_t *my_str) {
    for (int i = 0; my_str[i] != '\0'; i++) {
        uart_send(UARTx, my_str[i]);
    }
}