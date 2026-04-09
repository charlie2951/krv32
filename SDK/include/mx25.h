/*
Single-SPI Flash driver for macronix flash (4MB)
Designed for KRV-32 CPU
*/
#ifndef MX25_H
#define MX25_H

#include <stdint.h>
#include "spi.h"

// Macronix Commands
#define CMD_READ_ID          0x90
#define CMD_READ_DATA        0x03
#define CMD_WRITE_ENABLE     0x06
#define CMD_SECTOR_ERASE     0x20
#define CMD_PAGE_PROGRAM     0x02
#define CMD_READ_STATUS_1    0x05

// Macronix Identifiers
#define MACRONIX_MANU_ID     0xC2
#define MX25L32_DEVICE_ID    0x15 // Typical for 32Mb parts

// Function Prototypes
uint8_t  mx25_init(uint16_t clk_div);
uint16_t mx25_get_id(void);
void     mx25_read(uint32_t addr, uint8_t* buf, uint32_t len);
void     mx25_erase_sector(uint32_t addr);
void     mx25_write_page(uint32_t addr, uint8_t* data, uint16_t len);
void     mx25_wait_busy(void);

#endif