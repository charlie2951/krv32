#include "uart.h"
#include "delay.h"


//Initialize UART
void uart_init(UART_TypeDef *UARTx)
{
    UARTx->RXCTRL = 1;   // RX enable stays ON
}

//Function to send a character to UART
void uart_send(UART_TypeDef *UARTx, uint8_t my_char) {
    // Wait until the specific UART's status is ready
    while(UARTx->STATUS == 0); 
    
    UARTx->DATA = my_char;
    UARTx->CTRL = 1;
    UARTx->CTRL = 0;
}

// Function to read data from UART 
uint8_t uart_receive(UART_TypeDef *UARTx)
{
    /* Make sure RX is enabled (safe even if already enabled) */
    UARTx->RXCTRL = 1;
    UARTx->RXCTRL = 0; // Pulse
    /* Wait until a character is received */
    while (UARTx->RXSTATUS == 0) {
        ;   // blocking wait
    }
    /* Read received byte */
    uint8_t rx_char = (uint8_t)(UARTx->RXDATA & 0xFF);

    return rx_char;
}

// function to send string to UART
void uart_sendline(UART_TypeDef *UARTx, const uint8_t *my_str) {
    for (int i = 0; my_str[i] != '\0'; i++) {
        uart_send(UARTx, my_str[i]);
    }
}

//helper func replacement of snprintf()

 void uart_sendnumber(UART_TypeDef *UARTx,uint32_t val)
{
    static char tmp[12];
    int i = 0;

    
    do {
        tmp[i++] = '0' + (val % 10);
        val /= 10;
    } while (val);

    while (i--)
        uart_send(UARTx, tmp[i]);
}
