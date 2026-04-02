#ifndef BME280_H
#define BME280_H

#include "i2c.h"

#define BME280_ADDR             0x76  // Can be 0x77 depending on SDO pin
#define BME280_CHIP_ID          0xD0
#define BME280_REG_RESET        0xE0
#define BME280_REG_CTRL_HUM     0xF2
#define BME280_REG_STATUS       0xF3
#define BME280_REG_CTRL_MEAS    0xF4
#define BME280_REG_CONFIG       0xF5
#define BME280_REG_DATA         0xF7

// Calibration Data Structure
typedef struct {
    uint16_t dig_T1; int16_t dig_T2; int16_t dig_T3;
    uint16_t dig_P1; int16_t dig_P2; int16_t dig_P3; int16_t dig_P4;
    int16_t  dig_P5; int16_t dig_P6; int16_t dig_P7; int16_t dig_P8; int16_t dig_P9;
    uint8_t  dig_H1; int16_t dig_H2; uint8_t  dig_H3; int16_t dig_H4; int16_t dig_H5; int8_t dig_H6;
    int32_t  t_fine;
} bme280_calib_t;

// Function Prototypes
int8_t  bme280_init_i2c(i2c_t *i2c);
void bme280_i2c_read_fixed(i2c_t *i2c, int32_t *temp, uint32_t *press, uint32_t *hum);
void bme280_i2c_get_temperature(i2c_t *i2c, int32_t *temp);
void bme280_i2c_get_pressure(i2c_t *i2c, uint32_t *press);
void bme280_i2c_get_humidity(i2c_t *i2c, uint32_t *hum);
void bme280_i2c_trigger_forced(i2c_t *i2c) ;
int8_t bme280_init_spi(uint16_t clk_div);
void bme280_get_temperature_spi(int32_t *temp);
void bme280_get_pressure_spi(uint32_t *press);
void bme280_get_humidity_spi(uint32_t *hum);



#endif