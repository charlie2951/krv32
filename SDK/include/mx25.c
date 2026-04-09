#include "mx25.h"

/**
 * @brief Initialize SPI for Macronix Flash
 */
uint8_t mx25_init(uint16_t clk_div) {
    // Macronix supports SPI Mode 0 and Mode 3
    spi_init(SYS_SPI,clk_div, 0); //configured as  mode-0
    
    uint16_t id = mx25_get_id();
    
    // Validate Manufacturer ID (0xC2)
    if ((id >> 8) == MACRONIX_MANU_ID) {
        return 1; // Success
    }
    return 0; // Fail
}

uint16_t mx25_get_id(void) {
    uint8_t m_id, d_id;
    spi_cs_low(SYS_SPI);
    spi_transfer(SYS_SPI,CMD_READ_ID);
    // 24-bit dummy address 0x000000
    spi_transfer(SYS_SPI,0x00); 
    spi_transfer(SYS_SPI,0x00); 
    spi_transfer(SYS_SPI,0x00);
    m_id = spi_transfer(SYS_SPI,0xFF); // Receive 0xC2
    d_id = spi_transfer(SYS_SPI,0xFF); // Receive Device ID (e.g., 0x15)
    spi_cs_high(SYS_SPI);
    return (uint16_t)((m_id << 8) | d_id);
}

void mx25_wait_busy(void) {
    uint8_t status;
    do {
        spi_cs_low(SYS_SPI);
        spi_transfer(SYS_SPI,CMD_READ_STATUS_1);
        status = spi_transfer(SYS_SPI,0xFF);
        spi_cs_high(SYS_SPI);
        // Bit 0 is WIP (Write In Progress)
    } while (status & 0x01); 
}

void mx25_read(uint32_t addr, uint8_t* buf, uint32_t len) {
    spi_cs_low(SYS_SPI);
    spi_transfer(SYS_SPI,CMD_READ_DATA);
    // Send 3-byte address
    spi_transfer(SYS_SPI,(addr >> 16) & 0xFF);
    spi_transfer(SYS_SPI,(addr >> 8) & 0xFF);
    spi_transfer(SYS_SPI,addr & 0xFF);
    for (uint32_t i = 0; i < len; i++) {
        buf[i] = spi_transfer(SYS_SPI,0xFF);
    }
    spi_cs_high(SYS_SPI);
}

void mx25_erase_sector(uint32_t addr) {
    // Write Enable (WREN) must be sent before erase
    spi_cs_low(SYS_SPI); 
    spi_transfer(SYS_SPI,CMD_WRITE_ENABLE); 
    spi_cs_high(SYS_SPI);

    spi_cs_low(SYS_SPI);
    spi_transfer(SYS_SPI,CMD_SECTOR_ERASE);
    spi_transfer(SYS_SPI,(addr >> 16) & 0xFF);
    spi_transfer(SYS_SPI,(addr >> 8) & 0xFF);
    spi_transfer(SYS_SPI,addr & 0xFF);
    spi_cs_high(SYS_SPI);
    
    mx25_wait_busy(); 
}

void mx25_write_page(uint32_t addr, uint8_t* data, uint16_t len) {
    spi_cs_low(SYS_SPI); 
    spi_transfer(SYS_SPI,CMD_WRITE_ENABLE); 
    spi_cs_high(SYS_SPI);

    spi_cs_low(SYS_SPI);
    spi_transfer(SYS_SPI,CMD_PAGE_PROGRAM);
    spi_transfer(SYS_SPI,(addr >> 16) & 0xFF);
    spi_transfer(SYS_SPI,(addr >> 8) & 0xFF);
    spi_transfer(SYS_SPI,addr & 0xFF);
    
    for (uint16_t i = 0; i < len; i++) {
        spi_transfer(SYS_SPI,data[i]);
    }
    spi_cs_high(SYS_SPI);
    
    mx25_wait_busy();
}