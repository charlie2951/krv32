#ifndef QSPI_H
#define QSPI_H

#include <stdint.h>

// -----------------------------
// BASE ADDRESS
// -----------------------------
#define QSPI_BASE 0xC2000000

// -----------------------------
// REGISTER MAP
// -----------------------------
#define QSPI_REG_DATA   (*(volatile uint32_t*)(QSPI_BASE + 0x00))
#define QSPI_REG_CTRL   (*(volatile uint32_t*)(QSPI_BASE + 0x04))
#define QSPI_REG_ADDR   (*(volatile uint32_t*)(QSPI_BASE + 0x08))
#define QSPI_REG_STATUS (*(volatile uint32_t*)(QSPI_BASE + 0x0C))

// -----------------------------
// STATUS
// -----------------------------
#define QSPI_STATUS_BUSY   (1 << 0)
#define QSPI_STATUS_DONE   (1 << 1)

// -----------------------------
// CTRL FIELD
// -----------------------------
#define QSPI_CTRL_START        (1 << 0)
#define QSPI_CTRL_MODE_WR      (1 << 1)
#define QSPI_CTRL_MODE_RD      (0 << 1)
#define QSPI_CTRL_QUAD_ADDR    (1 << 2)
#define QSPI_CTRL_BURST_EN     (1 << 3)

#define QSPI_CTRL_DUMMY(x)     (((x) & 0xF) << 4)
#define QSPI_CTRL_CMD(x)       (((x) & 0xFF) << 8)

// -----------------------------
// FLASH COMMANDS
// -----------------------------
#define FLASH_CMD_WREN        0x06
#define FLASH_CMD_RDSR        0x05
#define FLASH_CMD_WRSR        0x01
#define FLASH_CMD_RDSR2       0x35
#define FLASH_CMD_WRSR2       0x31

#define FLASH_CMD_READ_ID     0x9F
#define FLASH_CMD_SECTOR_ER   0x20
#define FLASH_CMD_PAGE_PROG   0x02

#define FLASH_CMD_QUAD_READ   0x6B
#define FLASH_CMD_QUAD_IO_RD  0xEB

// -----------------------------
#define FLASH_SR_WIP   (1 << 0)
#define FLASH_SR_QE    (1 << 6)

// -----------------------------
#define FLASH_PAGE_SIZE 256

// -----------------------------
// API
// -----------------------------
void qspi_init(void);
void qspi_flash_init(void);

uint32_t qspi_read_id(void);
void qspi_wait_flash_ready(void);

void qspi_erase_sector(uint32_t addr);

void qspi_write_word(uint32_t addr, uint32_t data);
void qspi_write_buffer(uint32_t addr, const uint8_t *buf, uint32_t len);

uint32_t qspi_read_word_quad(uint32_t addr);
uint32_t qspi_read_word_quad_io(uint32_t addr);

#endif