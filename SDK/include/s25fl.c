#include "s25fl.h"

/**
 * @brief Initialize SPI and verify Spansion Flash ID
 */
uint8_t s25fl_init(uint16_t clk_div) {
    // Spansion S25FL supports SPI Mode 0 and Mode 3
    spi_init(SYS_SPI, clk_div, 0); 
    
    uint32_t id = s25fl_get_id();
    
    // Check Manufacturer ID (0x01) and Capacity Byte (0x18)
    if (((id >> 16) & 0xFF) == SPANSION_MANU_ID && (id & 0xFF) == S25FL128S_ID_BYTE2) {
        return 1; // Success
    }
    return 0; // Fail
}

/**
 * @brief Uses JEDEC Read ID (9Fh) as Spansion parts often 
 * use this for more reliable identification than 90h.
 */
uint32_t s25fl_get_id(void) {
    uint8_t m_id, type, cap;
    spi_cs_low(SYS_SPI);
    spi_transfer(SYS_SPI, CMD_READ_ID);
    
    m_id = spi_transfer(SYS_SPI, 0xFF); // Manufacturer ID (0x01)
    type = spi_transfer(SYS_SPI, 0xFF); // Device ID Byte 1
    cap  = spi_transfer(SYS_SPI, 0xFF); // Device ID Byte 2 (0x18)
    
    spi_cs_high(SYS_SPI);
    return (uint32_t)((m_id << 16) | (type << 8) | cap);
}

void s25fl_wait_busy(void) {
    uint8_t status;
    do {
        spi_cs_low(SYS_SPI);
        spi_transfer(SYS_SPI, CMD_READ_STATUS_1);
        status = spi_transfer(SYS_SPI, 0xFF);
        spi_cs_high(SYS_SPI);
        // Bit 0 is WIP (Write In Progress)
    } while (status & 0x01); 
}

void s25fl_read(uint32_t addr, uint8_t* buf, uint32_t len) {
    spi_cs_low(SYS_SPI);
    spi_transfer(SYS_SPI, CMD_READ_DATA);
    // Send 3-byte address (For 128Mb/16MB, 24-bit is sufficient)
    spi_transfer(SYS_SPI, (addr >> 16) & 0xFF);
    spi_transfer(SYS_SPI, (addr >> 8) & 0xFF);
    spi_transfer(SYS_SPI, addr & 0xFF);
    
    for (uint32_t i = 0; i < len; i++) {
        buf[i] = spi_transfer(SYS_SPI, 0xFF);
    }
    spi_cs_high(SYS_SPI);
}

void s25fl_erase_sector(uint32_t addr) {
    spi_cs_low(SYS_SPI); 
    spi_transfer(SYS_SPI, CMD_WRITE_ENABLE); 
    spi_cs_high(SYS_SPI);

    spi_cs_low(SYS_SPI);
    spi_transfer(SYS_SPI, CMD_SECTOR_ERASE);
    spi_transfer(SYS_SPI, (addr >> 16) & 0xFF);
    spi_transfer(SYS_SPI, (addr >> 8) & 0xFF);
    spi_transfer(SYS_SPI, addr & 0xFF);
    spi_cs_high(SYS_SPI);
    
    s25fl_wait_busy(); 
}

void s25fl_write_page(uint32_t addr, uint8_t* data, uint16_t len) {
    spi_cs_low(SYS_SPI); 
    spi_transfer(SYS_SPI, CMD_WRITE_ENABLE); 
    spi_cs_high(SYS_SPI);

    spi_cs_low(SYS_SPI);
    spi_transfer(SYS_SPI, CMD_PAGE_PROGRAM);
    spi_transfer(SYS_SPI, (addr >> 16) & 0xFF);
    spi_transfer(SYS_SPI, (addr >> 8) & 0xFF);
    spi_transfer(SYS_SPI, addr & 0xFF);
    
    for (uint16_t i = 0; i < len; i++) {
        spi_transfer(SYS_SPI, data[i]);
    }
    spi_cs_high(SYS_SPI);
    
    s25fl_wait_busy();
}