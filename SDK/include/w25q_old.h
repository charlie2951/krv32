#ifndef W25Q_H
#define W25Q_H

#include <stdint.h>
#include "spi.h"

// W25Q Commands
#define CMD_READ_ID         0x90
#define CMD_READ_DATA       0x03
#define CMD_WRITE_ENABLE    0x06
#define CMD_SECTOR_ERASE    0x20
#define CMD_PAGE_PROGRAM    0x02
#define CMD_READ_STATUS_1   0x05

// Function Prototypes
uint16_t w25q_get_id(void);
void w25q_read(uint32_t addr, uint8_t* buf, uint32_t len);
void w25q_erase_sector(uint32_t addr);
void w25q_write_page(uint32_t addr, uint8_t* data, uint16_t len);
void w25q_wait_busy(void);

#endif