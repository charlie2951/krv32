#include "w25q.h"

uint16_t w25q_get_id(void) {
    uint8_t m_id, d_id;
    spi_cs_low();
    spi_transfer(CMD_READ_ID);
    spi_transfer(0x00); spi_transfer(0x00); spi_transfer(0x00);
    m_id = spi_transfer(0xFF);
    d_id = spi_transfer(0xFF);
    spi_cs_high();
    return (uint16_t)((m_id << 8) | d_id);
}

void w25q_wait_busy(void) {
    uint8_t status;
    do {
        spi_cs_low();
        spi_transfer(CMD_READ_STATUS_1);
        status = spi_transfer(0xFF);
        spi_cs_high();
    } while (status & 0x01); // Poll WIP bit
}

void w25q_read(uint32_t addr, uint8_t* buf, uint32_t len) {
    spi_cs_low();
    spi_transfer(CMD_READ_DATA);
    spi_transfer((addr >> 16) & 0xFF);
    spi_transfer((addr >> 8) & 0xFF);
    spi_transfer(addr & 0xFF);
    for (uint32_t i = 0; i < len; i++) buf[i] = spi_transfer(0xFF);
    spi_cs_high();
}

void w25q_erase_sector(uint32_t addr) {
    spi_cs_low(); spi_transfer(CMD_WRITE_ENABLE); spi_cs_high();
    spi_cs_low();
    spi_transfer(CMD_SECTOR_ERASE);
    spi_transfer((addr >> 16) & 0xFF);
    spi_transfer((addr >> 8) & 0xFF);
    spi_transfer(addr & 0xFF);
    spi_cs_high();
    w25q_wait_busy();
}

void w25q_write_page(uint32_t addr, uint8_t* data, uint16_t len) {
    spi_cs_low(); spi_transfer(CMD_WRITE_ENABLE); spi_cs_high();
    spi_cs_low();
    spi_transfer(CMD_PAGE_PROGRAM);
    spi_transfer((addr >> 16) & 0xFF);
    spi_transfer((addr >> 8) & 0xFF);
    spi_transfer(addr & 0xFF);
    for (uint16_t i = 0; i < len; i++) spi_transfer(data[i]);
    spi_cs_high();
    w25q_wait_busy();
}