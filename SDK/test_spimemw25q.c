#include "uart.h"
#include "spi.h"
#include "w25q.h"

void byte2hex(uint8_t val, char *dest) {
    static const char hex_table[] = "0123456789ABCDEF";
    dest[0] = hex_table[(val >> 4) & 0x0F];
    dest[1] = hex_table[val & 0x0F];
    dest[2] = '\0';
}

void print_hex_byte(uint8_t val) {
    char hex[3];
    byte2hex(val, hex);
    uart_sendline(UART1, "0x");
    uart_sendline(UART1, hex);
    uart_sendline(UART1, " ");
}

int main(void) {
    w25q_init(10);//100MHz divided by 10=10MHz SPI clock
    uart_sendline(UART1, "\r\n--- RISC-V Flash System Ready ---\r\n");

    uint16_t id = w25q_get_id();
    uart_sendline(UART1, "ID: ");
    print_hex_byte(id >> 8);
    uart_sendline(UART1, " ");
    print_hex_byte(id & 0xFF);
    uart_sendline(UART1, "\r\n");

    uint8_t test_data[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    uint8_t rx_buf[4];

    uart_sendline(UART1, "Erasing and Writing...\r\n");
    w25q_erase_sector(0x000000);
    w25q_write_page(0x000000, test_data, 4);

    uart_sendline(UART1, "Verifying: ");
    w25q_read(0x000000, rx_buf, 4);
    for(int i=0; i<4; i++) print_hex_byte(rx_buf[i]);

    while(1);
    return 0;
}