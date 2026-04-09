#ifndef BME280_SPI_H
#define BME280_SPI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// =====================================================
// BME280 SPI Register Map
// =====================================================
#define BME280_CHIP_ID          0xD0
#define BME280_RESET            0xE0
#define BME280_REG_STATUS       0xF3
#define BME280_REG_CTRL_MEAS    0xF4
#define BME280_REG_CONFIG       0xF5
#define BME280_REG_CTRL_HUM     0xF2

// Data Registers
#define BME280_REG_PRESS_MSB    0xF7
#define BME280_REG_TEMP_MSB     0xFA
#define BME280_REG_HUM_MSB      0xFD

// =====================================================
// Calibration Structure
// =====================================================
typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;

    int32_t  t_fine; // Internal variable for compensation
} bme280_calib_t;

// =====================================================
// Function Prototypes
// =====================================================

/**
 * @brief Initializes the BME280 over SPI.
 * @param clk_div Clock divider for the SPI peripheral.
 * @return 0 on success, -1 if Chip ID mismatch.
 */
int8_t bme280_init_spi(uint16_t clk_div);

/**
 * @brief Reads temperature in Forced Mode.
 * @param temp Pointer to store result (in 0.01 degC, e.g., 2505 = 25.05 C).
 */
void bme280_get_temperature_spi(int32_t *temp);

/**
 * @brief Reads pressure in Forced Mode.
 * @param press Pointer to store result in Pascals (Pa).
 */
void bme280_get_pressure_spi(uint32_t *press);

/**
 * @brief Reads humidity in Forced Mode.
 * @param hum Pointer to store result in %RH (Q22.10 format).
 */
void bme280_get_humidity_spi(uint32_t *hum);

#ifdef __cplusplus
}
#endif

#endif // BME280_SPI_H