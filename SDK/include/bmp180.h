#ifndef BMP180_H
#define BMP180_H

#include "i2c.h"
#include <stdint.h>

#define BMP180_ADDR           0x77
#define BMP180_CHIP_ID_REG    0xD0
#define BMP180_VERSION_REG    0xD1
#define BMP180_CTRL_MEAS_REG  0xF4
#define BMP180_ADC_OUT_MSB    0xF6

// Commands
#define BMP180_GET_TEMP       0x2E
#define BMP180_GET_PRESSURE   0x34 // Base, depends on oversampling
// Constants for Bhubaneswar / Standard Sea Level
#define SEA_LEVEL_PRESSURE 101325 


// Oversampling Settings (OSS)
typedef enum {
    BMP180_LOW_POWER = 0,
    BMP180_STANDARD = 1,
    BMP180_HIGH_RES = 2,
    BMP180_ULTRA_HIGH_RES = 3
} bmp180_oss_t;

// Calibration Coefficients (Read from EEPROM)
typedef struct {
    int16_t ac1, ac2, ac3;
    uint16_t ac4, ac5, ac6;
    int16_t b1, b2, mb, mc, md;
} bmp180_calib_t;

/**
 * @brief Init sensor and read calibration coefficients.
 * @return 0 on success, -1 if ID mismatch.
 */
int bmp180_init(i2c_t *i2c);

/**
 * @brief Read uncompensated temperature.
 */
int32_t bmp180_get_raw_temp(i2c_t *i2c);

/**
 * @brief Read uncompensated pressure.
 */
int32_t bmp180_get_raw_pressure(i2c_t *i2c, bmp180_oss_t oss);

/**
 * @brief Calculate true temperature in Celsius (x10).
 */
int32_t bmp180_calculate_temp(int32_t ut);

/**
 * @brief Calculate true pressure in Pascals.
 */
int32_t bmp180_calculate_pressure(int32_t up, bmp180_oss_t oss);
int32_t bmp180_get_altitude(int32_t pressure, int32_t baseline_pressure);
#endif