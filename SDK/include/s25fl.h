/*
Single-SPI Flash driver for Spansion S25FL128S (16MB)
Target Hardware: Nexys 4 DDR (Artix-7)
Designed for KRV-32 CPU
*/
#ifndef S25FL_H
#define S25FL_H

#include <stdint.h>
#include "spi.h"

// Spansion S25FL Commands
#define CMD_READ_ID          0x9F // Use JEDEC ID for Spansion
#define CMD_READ_DATA        0x03
#define CMD_WRITE_ENABLE     0x06
#define CMD_SECTOR_ERASE     0xD8 // 64KB Sector Erase (standard for S25FL)
#define CMD_PAGE_PROGRAM     0x02
#define CMD_READ_STATUS_1    0x05

// Spansion Identifiers
#define SPANSION_MANU_ID     0x01
#define S25FL128S_ID_BYTE1   0x20 // Memory type
#define S25FL128S_ID_BYTE2   0x18 // Capacity (128Mb)

// Function Prototypes
uint8_t  s25fl_init(uint16_t clk_div);
uint32_t s25fl_get_id(void);
void     s25fl_read(uint32_t addr, uint8_t* buf, uint32_t len);
void     s25fl_erase_sector(uint32_t addr);
void     s25fl_write_page(uint32_t addr, uint8_t* data, uint16_t len);
void     s25fl_wait_busy(void);

#endif