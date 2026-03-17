#ifndef ADXL345_H
#define ADXL345_H

#include <stdint.h>
#include "spi.h"
#include "i2c.h"
// --- I2C Slave Addresses ---
#define ADXL345_ADDR          0x53    // SDO tied to GND
#define ADXL345_ADDR_ALT      0x1D    // SDO tied to VCC
// ADXL345 Register Map
#define ADXL345_REG_DEVID          0x00    
#define ADXL345_REG_POWER_CTL      0x2D    
#define ADXL345_REG_DATA_FORMAT    0x31    
#define ADXL345_REG_DATAX0         0x32 
#define ADXL345_REG_BW_RATE      0x2C   
// --- Useful Constants ---
#define ADXL345_DEVICE_ID        0xE5
#define ADXL345_MG_PER_LSB       0.0039f // 3.9mg/LSB in Full Res mode
// SPI Bit Masks
#define ADXL345_SPI_READ           0x80    
#define ADXL345_SPI_MB             0x40    

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} adxl345_data_t;

// Function Prototypes
uint8_t adxl345_spi_init(uint16_t clk_div); // Pass divider from main
int adxl345_i2c_init(i2c_t *i2c);
void adxl345_read_accel(adxl345_data_t* data);
uint8_t adxl345_read_reg(uint8_t reg);
void adxl345_write_reg(uint8_t reg, uint8_t val);
void adxl345_i2c_read(i2c_t *i2c, adxl345_data_t *data);
#endif