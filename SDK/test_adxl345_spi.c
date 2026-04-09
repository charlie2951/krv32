#include "uart.h"
#include "spi.h"
#include "adxl345.h"
#include "delay.h"

// Define our SPI Configuration
// If System Clock = 100MHz, Divider 50 = 2MHz SPI Clock
#define SPI_CLOCK_DIVIDER 50 

int main(void) {
    adxl345_data_t accel;

    // Optional: Initialize UART first so we can see debug messages
    // Assuming a standard uart_init function exists
    // uart_init(UART1, 115200); 

    uart_sendline(UART1, "--- SPI Universal Master Test ---\r\n");

    /* adxl_init() now performs:
       1. spi_init(SPI_CLOCK_DIVIDER, 3) -> Mode 3 is required for ADXL345
       2. Device ID verification (0xE5)
       3. Power and Format configuration
    */
    if (adxl345_spi_init(SPI_CLOCK_DIVIDER)) {
        uart_sendline(UART1, "ADXL345 Initialized Successfully!\r\n");
    } else {
        uart_sendline(UART1, "ADXL345 Error: ID Mismatch or Hardware Timeout\r\n");
        // Loop here on error
        while(1);
    }

    while(1) {
        // Read X, Y, Z data in one multi-byte SPI burst
        adxl345_read_accel(&accel);
        
        // Output formatted data
        uart_sendline(UART1, "X: ");
        uart_sendnumber(UART1, (int32_t)accel.x);
        uart_sendline(UART1, "  Y: ");
        uart_sendnumber(UART1, (int32_t)accel.y);
        uart_sendline(UART1, "  Z: ");
        uart_sendnumber(UART1, (int32_t)accel.z);
        uart_sendline(UART1, "\r\n");

        // Wait approx 500ms between samples
        delay(500000); 
    }

    return 0;
}