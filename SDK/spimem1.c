#include <stdint.h>
#include "uart.h"

// Register Definitions
#define SPI_REG_CTRL    (*((volatile uint32_t*)0xC1000000))
#define SPI_REG_DATA    (*((volatile uint32_t*)0xC1000004))

// Bit Masks
#define SPI_START       (1 << 0)
#define SPI_BUSY        (1 << 1)
#define SPI_CS_BIT      (1 << 2)

/**
 * Robust SPI Byte Transfer
 * Uses the 'Sticky' handshake logic.
 */
uint8_t spi_transfer_byte(uint8_t out_byte) {
    // 1. Clear start bit just in case it was set
    SPI_REG_CTRL &= ~SPI_START;
    
    // 2. Load data
    SPI_REG_DATA = (uint32_t)out_byte;
    
    // 3. Set Start HIGH
    SPI_REG_CTRL |= SPI_START;
    
    // 4. Wait for FSM to leave IDLE (acknowledge start)
    for(volatile int i = 0; i < 20; i++); 
    
    // 5. Wait for the SPI clocking to finish 8 bits
    // Based on CLK_DIV 50, this provides a safe margin.
    for(volatile int i = 0; i < 2500; i++); 

    // 6. Read the received data
    uint8_t in_byte = (uint8_t)(SPI_REG_DATA & 0xFF);
    
    // 7. Set Start LOW to release FSM back to IDLE
    SPI_REG_CTRL &= ~SPI_START;
    
    // 8. Wait for busy bit to actually drop
    while (SPI_REG_CTRL & SPI_BUSY);

    return in_byte;
}

uint16_t w25q_get_id(void) {
    uint8_t m_id, d_id;

    SPI_REG_CTRL &= ~SPI_CS_BIT; // CS Low (Select)

    spi_transfer_byte(0x90);     // Command
    spi_transfer_byte(0x00);     // Addr byte 1
    spi_transfer_byte(0x00);     // Addr byte 2
    spi_transfer_byte(0x00);     // Addr byte 3

    m_id = spi_transfer_byte(0xFF); // Read Manuf ID
    d_id = spi_transfer_byte(0xFF); // Read Device ID

    SPI_REG_CTRL |= SPI_CS_BIT;  // CS High (Deselect)

    return (uint16_t)((m_id << 8) | d_id);
}
void w25q_data_read(uint32_t addr, uint8_t* buffer, uint32_t len) {
    SPI_REG_CTRL &= ~SPI_CS_BIT; // CS Low

    spi_transfer_byte(0x03);           // Read Command
    spi_transfer_byte((addr >> 16) & 0xFF); // Addr High
    spi_transfer_byte((addr >> 8) & 0xFF);  // Addr Mid
    spi_transfer_byte(addr & 0xFF);         // Addr Low

    for (uint32_t i = 0; i < len; i++) {
        buffer[i] = spi_transfer_byte(0xFF); // Clock out data
    }

    SPI_REG_CTRL |= SPI_CS_BIT;  // CS High
}
void w25q_wait_for_idle(void) {
    uint8_t status = 0;
    do {
        SPI_REG_CTRL &= ~SPI_CS_BIT;      // CS Low
        spi_transfer_byte(0x05);          // Read Status Register 1
        status = spi_transfer_byte(0xFF); // Get status byte
        SPI_REG_CTRL |= SPI_CS_BIT;       // CS High
    } while (status & 0x01);              // Bit 0 is WIP (Write In Progress)
}
void w25q_sector_erase(uint32_t addr) {
    // 1. Enable Writing
    SPI_REG_CTRL &= ~SPI_CS_BIT;
    spi_transfer_byte(0x06); 
    SPI_REG_CTRL |= SPI_CS_BIT;

    // 2. Send Erase Command
    SPI_REG_CTRL &= ~SPI_CS_BIT;
    spi_transfer_byte(0x20);              // Sector Erase (20h)
    spi_transfer_byte((addr >> 16) & 0xFF);
    spi_transfer_byte((addr >> 8) & 0xFF);
    spi_transfer_byte(addr & 0xFF);
    SPI_REG_CTRL |= SPI_CS_BIT;

    // 3. Wait for chip to finish erasing
    w25q_wait_for_idle();
}
void w25q_page_write(uint32_t addr, uint8_t* data, uint16_t len) {
    // 1. Enable Writing
    SPI_REG_CTRL &= ~SPI_CS_BIT;
    spi_transfer_byte(0x06); 
    SPI_REG_CTRL |= SPI_CS_BIT;

    // 2. Send Program Command
    SPI_REG_CTRL &= ~SPI_CS_BIT;
    spi_transfer_byte(0x02);              // Page Program (02h)
    spi_transfer_byte((addr >> 16) & 0xFF);
    spi_transfer_byte((addr >> 8) & 0xFF);
    spi_transfer_byte(addr & 0xFF);

    // 3. Send Data
    for (uint16_t i = 0; i < len; i++) {
        spi_transfer_byte(data[i]);
    }
    SPI_REG_CTRL |= SPI_CS_BIT;

    // 4. Wait for chip to finish programming
    w25q_wait_for_idle();
}
void byte2hex(uint8_t val, char *dest) {
    static const char hex_table[] = "0123456789ABCDEF";
    dest[0] = hex_table[(val >> 4) & 0x0F];
    dest[1] = hex_table[val & 0x0F];
    dest[2] = '\0';
}

int main(void) {
    // --- 1. MANDATORY HARDWARE SANITIZATION ---
    // Force everything to a known state immediately on reset
    SPI_REG_CTRL &= ~SPI_START;  // Force start bit off
    SPI_REG_CTRL |= SPI_CS_BIT;   // Ensure CS is high (Deselect)
    
    // Wait for any pending hardware cycles to clear
    while (SPI_REG_CTRL & SPI_BUSY);

    // Short stability delay
    for(volatile int i = 0; i < 500000; i++);

    // --- 2. UART BOOT MESSAGE ---
    // This helps us see if the reset button actually restarted the CPU
    uart_sendline(UART1, "\r\n--- RISC-V SPI Boot ---\r\n");

    // --- 3. EXECUTION ---
    uint16_t chip_id = w25q_get_id();

    uint8_t manufacturer = (uint8_t)(chip_id >> 8);
    uint8_t device       = (uint8_t)(chip_id & 0xFF);

    uart_sendline(UART1, "Flash Found. ID: ");
    uart_sendnumber(UART1, manufacturer);
    uart_sendline(UART1, " ");
    uart_sendnumber(UART1, device);
    uart_sendline(UART1, "\r\n");
//
uint8_t my_data[4] = {0xDE, 0xAD, 0xBE, 0xEF};
uint8_t read_back[4];
char hex_str[3];//store converted hex data
// 1. Erase sector at address 0
w25q_sector_erase(0x000000);
uart_sendline(UART1,"Erasing flash!\r\n");
// 2. Write data
w25q_page_write(0x000000, my_data, 4);
uart_sendline(UART1,"Writing data to flash...\r\n");
// 3. Read back to verify (using the read function from previous step)
w25q_data_read(0x000000, read_back, 4);
uart_sendline(UART1,"Reading back data from flash...\r\n");
//print read data
for(uint8_t i=0; i<4; i++){
    byte2hex(read_back[i], hex_str);
    uart_sendline(UART1,"0x");
    uart_sendline(UART1, hex_str);
    uart_sendline(UART1, " ");
}

    while (1) ;

    return 0;
}