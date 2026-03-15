#ifndef SOFT_W25Q_H
#define SOFT_W25Q_H

#include <stdint.h>
#include "soft_spi.h"

// Flash Commands
#define CMD_WREN           0x06
#define CMD_RDSR1          0x05
#define CMD_READ           0x03
#define CMD_PAGE_PROG      0x02
#define CMD_SECTOR_ERASE   0x20
#define CMD_JEDEC_ID       0x9F

// Prototypes
void w25q_read_id(uint8_t *id_buf);
void w25q_read_data(uint32_t addr, uint8_t *buffer, uint32_t len);
void w25q_erase_sector(uint32_t addr);
void w25q_write_page(uint32_t addr, uint8_t *data, uint16_t len);
void byte_to_hex(uint8_t val, char *dest);

#endif