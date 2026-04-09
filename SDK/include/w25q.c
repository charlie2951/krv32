#include "w25q.h"
#define SPI_PORT SPI0
/**
 * @brief Initialize SPI for W25Q Flash
 * @param clk_div: Divider value (W25Q usually supports up to 80-104MHz, 
 * so clk_div=2 or 4 is common for 100MHz systems).
 */
uint8_t w25q_init(uint16_t clk_div) {
    // W25Q works best in Mode 0 (CPOL=0, CPHA=0)
    spi_init(SPI_PORT,clk_div, 0); 
    
    // Perform a quick ID check to verify connection
    uint16_t id = w25q_get_id();
    
    // Typical Winbond Manufacturer ID is 0xEF
    if ((id >> 8) == 0xEF) {
        return 1; // Success
    }
    return 0; // Fail
}

uint16_t w25q_get_id(void) {
    uint8_t m_id, d_id;
    spi_cs_low(SPI_PORT);
    spi_transfer(SPI_PORT,CMD_READ_ID);
    // Address 0x000000 follows the Read ID command
    spi_transfer(SPI_PORT,0x00); 
    spi_transfer(SPI_PORT,0x00); 
    spi_transfer(SPI_PORT,0x00);
    m_id = spi_transfer(SPI_PORT,0xFF); // Receive Manufacturer ID
    d_id = spi_transfer(SPI_PORT,0xFF); // Receive Device ID
    spi_cs_high(SPI_PORT);
    return (uint16_t)((m_id << 8) | d_id);
}

void w25q_wait_busy(void) {
    uint8_t status;
    do {
        spi_cs_low(SPI_PORT);
        spi_transfer(SPI_PORT,CMD_READ_STATUS_1);
        status = spi_transfer(SPI_PORT,0xFF);
        spi_cs_high(SPI_PORT);
        // Bit 0 is the BUSY bit (1 = writing/erasing)
    } while (status & 0x01); 
}

void w25q_read(uint32_t addr, uint8_t* buf, uint32_t len) {
    spi_cs_low(SPI_PORT);
    spi_transfer(SPI_PORT,CMD_READ_DATA);
    spi_transfer(SPI_PORT,(addr >> 16) & 0xFF);
    spi_transfer(SPI_PORT,(addr >> 8) & 0xFF);
    spi_transfer(SPI_PORT,addr & 0xFF);
    for (uint32_t i = 0; i < len; i++) {
        buf[i] = spi_transfer(SPI_PORT,0xFF);
    }
    spi_cs_high(SPI_PORT);
}

void w25q_erase_sector(uint32_t addr) {
    // Write Enable must be pulsed before an erase
    spi_cs_low(SPI_PORT); 
    spi_transfer(SPI_PORT,CMD_WRITE_ENABLE); 
    spi_cs_high(SPI_PORT);

    spi_cs_low(SPI_PORT);
    spi_transfer(SPI_PORT,CMD_SECTOR_ERASE);
    spi_transfer(SPI_PORT,(addr >> 16) & 0xFF);
    spi_transfer(SPI_PORT,(addr >> 8) & 0xFF);
    spi_transfer(SPI_PORT,addr & 0xFF);
    spi_cs_high(SPI_PORT);
    
    w25q_wait_busy(); // Erasure takes time (typically 40ms - 400ms)
}

void w25q_write_page(uint32_t addr, uint8_t* data, uint16_t len) {
    spi_cs_low(SPI_PORT); 
    spi_transfer(SPI_PORT,CMD_WRITE_ENABLE); 
    spi_cs_high(SPI_PORT);

    spi_cs_low(SPI_PORT);
    spi_transfer(SPI_PORT,CMD_PAGE_PROGRAM);
    spi_transfer(SPI_PORT,(addr >> 16) & 0xFF);
    spi_transfer(SPI_PORT,(addr >> 8) & 0xFF);
    spi_transfer(SPI_PORT,addr & 0xFF);
    
    for (uint16_t i = 0; i < len; i++) {
        spi_transfer(SPI_PORT,data[i]);
    }
    spi_cs_high(SPI_PORT);
    
    w25q_wait_busy(); // Page program typically takes ~0.7ms
}