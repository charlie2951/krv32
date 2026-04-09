#ifndef SOFT_ADXL345_H
#define SOFT_ADXL345_H

#include <stdint.h>
#include "soft_spi.h"

// ADXL345 Register Addresses
#define ADXL_REG_DEVID          0x00
#define ADXL_REG_POWER_CTL      0x2D
#define ADXL_REG_DATA_FORMAT    0x31
#define ADXL_REG_DATAX0         0x32 // Data starts here (6 bytes total)

// Protocol Bits
#define ADXL_READ_BIT           (1 << 7)
#define ADXL_MB_BIT             (1 << 6) // Multi-Byte bit

typedef struct {
    int16_t x, y, z;
} soft_adxl_data_t;

// Function Prototypes
uint8_t soft_adxl_init(void);
uint8_t soft_adxl_read_reg(uint8_t reg);
void    soft_adxl_write_reg(uint8_t reg, uint8_t val);
void    soft_adxl_read_accel(soft_adxl_data_t* data);

#endif