#include "soft_spi.h"
#include "soft_w25q.h"
#include "uart.h"

//This flash module should be connected to GPIO as mentioned below
/*
PIN_SCLK (GPIO-0)
PIN_MOSI (GPIO-1)
PIN_MISO (GPIO-2)
PIN_CS   (GPIO-3)
*/
int main() {
    uint32_t target_addr = 0x001000;
    uint8_t write_buf[4] = {0x12, 0x34, 0x56, 0x78};//sample data to write
    uint8_t read_buf[4]  = {0};
    uint8_t ids[3];
    char hex_str[3];

    spi_bb_init();
    
    // Test Erase/Write/Read
    soft_w25q_erase_sector(target_addr);
    soft_w25q_write_page(target_addr, write_buf, 4);
    soft_w25q_read_data(target_addr, read_buf, 4);

    while (1) {
        soft_w25q_read_id(ids);
        uart_sendline(UART1, "\r\nDevice ID: ");
        byte_to_hex(ids[0], hex_str);
        uart_sendline(UART1, hex_str);
        
        uart_sendline(UART1, "\r\nFlash Content: ");
        for(int j = 0; j < 4; j++){
            byte_to_hex(read_buf[j], hex_str);
            uart_sendline(UART1, hex_str);
            uart_sendline(UART1, " ");
        }

        for (volatile int i = 0; i < 5000000; i++);
    }
    return 0;
}