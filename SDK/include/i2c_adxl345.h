#ifndef I2C_ADXL345_H
#define I2C_ADXL345_H

#include "i2c.h"
#include <stdint.h>

// --- I2C Slave Addresses ---
#define ADXL345_ADDR          0x53    // SDO tied to GND
#define ADXL345_ADDR_ALT      0x1D    // SDO tied to VCC

// --- Register Map ---
#define ADXL345_REG_DEVID        0x00
#define ADXL345_REG_BW_RATE      0x2C
#define ADXL345_REG_POWER_CTL    0x2D
#define ADXL345_REG_DATA_FORMAT  0x31
#define ADXL345_REG_DATAX0       0x32

// --- Useful Constants ---
#define ADXL345_DEVICE_ID        0xE5
#define ADXL345_MG_PER_LSB       0.0039f // 3.9mg/LSB in Full Res mode

// Data structure to hold raw axis values
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} adxl345_data_t;

/**
 * @brief Initializes the ADXL345 sensor over I2C.
 * @return 0 on success, -1 if Device ID mismatch.
 */
int adxl345_init(i2c_t *i2c);

/**
 * @brief Reads raw X, Y, Z acceleration data.
 */
void adxl345_read(i2c_t *i2c, adxl345_data_t *data);

#endif // ADXL345_H