#include "soft_w25q.h"

static void soft_w25q_write_enable() {
    spi_bb_cs_low();
    spi_bb_transfer(CMD_WREN);
    spi_bb_cs_high();
}

static void soft_w25q_wait_busy() {
    uint8_t status;
    do {
        spi_bb_cs_low();
        spi_bb_transfer(CMD_RDSR1);
        status = spi_bb_transfer(0xFF);
        spi_bb_cs_high();
    } while (status & 0x01);
}

void soft_w25q_read_id(uint8_t *id_buf) {
    spi_bb_cs_low();
    spi_bb_transfer(CMD_JEDEC_ID);
    id_buf[0] = spi_bb_transfer(0xFF);
    id_buf[1] = spi_bb_transfer(0xFF);
    id_buf[2] = spi_bb_transfer(0xFF);
    spi_bb_cs_high();
}

void soft_w25q_read_data(uint32_t addr, uint8_t *buffer, uint32_t len) {
    spi_bb_cs_low();
    spi_bb_transfer(CMD_READ);
    spi_bb_transfer((addr >> 16) & 0xFF);
    spi_bb_transfer((addr >> 8) & 0xFF);
    spi_bb_transfer(addr & 0xFF);
    for (uint32_t i = 0; i < len; i++) buffer[i] = spi_bb_transfer(0xFF);
    spi_bb_cs_high();
}

void soft_w25q_erase_sector(uint32_t addr) {
    soft_w25q_write_enable();
    spi_bb_cs_low();
    spi_bb_transfer(CMD_SECTOR_ERASE);
    spi_bb_transfer((addr >> 16) & 0xFF);
    spi_bb_transfer((addr >> 8) & 0xFF);
    spi_bb_transfer(addr & 0xFF);
    spi_bb_cs_high();
    soft_w25q_wait_busy();
}

void soft_w25q_write_page(uint32_t addr, uint8_t *data, uint16_t len) {
    soft_w25q_write_enable();
    spi_bb_cs_low();
    spi_bb_transfer(CMD_PAGE_PROG);
    spi_bb_transfer((addr >> 16) & 0xFF);
    spi_bb_transfer((addr >> 8) & 0xFF);
    spi_bb_transfer(addr & 0xFF);
    for (uint16_t i = 0; i < len; i++) spi_bb_transfer(data[i]);
    spi_bb_cs_high();
    soft_w25q_wait_busy();
}

void byte_to_hex(uint8_t val, char *dest) {
    static const char hex_table[] = "0123456789ABCDEF";
    dest[0] = hex_table[(val >> 4) & 0x0F];
    dest[1] = hex_table[val & 0x0F];
    dest[2] = '\0';
}