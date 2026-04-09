#include "uart.h"
#include "soft_adxl345.h"
/*
#include "soft_spi.h"
void debug_loopback() {
    spi_bb_init();
    spi_bb_cs_low();
    uint8_t test = spi_bb_transfer(0xAA); // Send 10101010
    spi_bb_cs_high();
    
    uart_sendline(UART1, "Loopback Result: ");
    uart_sendnumber(UART1, test); // Should print 170 (0xAA)
}
*/
int main(void) {
    soft_adxl_data_t accel;
    //debug_loopback();
    uart_sendline(UART1, "\r\n--- ADXL345 Bit-Bang Test ---\r\n");

    uint8_t result = soft_adxl_init();

    if (result == 0xE5) {
        uart_sendline(UART1, "Sensor detected successfully!\r\n");
    } else {
        uart_sendline(UART1, "Error! Received ID: ");
        uart_sendnumber(UART1, result); // This helps you see if it's 0 or 255
        uart_sendline(UART1, "\r\n");
        while(1);
    }

    while(1) {
        soft_adxl_read_accel(&accel);

        uart_sendline(UART1, "X: ");
        uart_sendnumber(UART1, accel.x);
        uart_sendline(UART1, "  Y: ");
        uart_sendnumber(UART1, accel.y);
        uart_sendline(UART1, "  Z: ");
        uart_sendnumber(UART1, accel.z);
        uart_sendline(UART1, "\r\n");

        for(volatile int i = 0; i < 1000000; i++); // Sample delay
    }
}