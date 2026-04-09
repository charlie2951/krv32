#include "bme280.h"
#include "spi.h"

static bme280_calib_t calib;

// =====================================================
// Internal SPI Helpers
// =====================================================

static void bme280_write_reg_spi(uint8_t reg, uint8_t value) {
    // SPI Write: MSB of address must be 0
    uint8_t addr = reg & 0x7F; 
    
    spi_cs_low();
    spi_transfer(addr);
    spi_transfer(value);
    spi_cs_high();
}

static uint8_t bme280_read_reg_spi(uint8_t reg) {
    // SPI Read: MSB of address must be 1
    uint8_t addr = reg | 0x80; 
    uint8_t data;

    spi_cs_low();
    spi_transfer(addr);
    data = spi_transfer(0x00); // Send dummy byte to receive data
    spi_cs_high();

    return data;
}

static uint16_t read16_spi(uint8_t reg) {
    uint8_t lsb, msb;
    uint8_t addr = reg | 0x80;

    spi_cs_low();
    spi_transfer(addr);
    lsb = spi_transfer(0x00); // Read register 'reg'
    msb = spi_transfer(0x00); // Read register 'reg + 1' (auto-increment)
    spi_cs_high();

    return (uint16_t)(lsb | (msb << 8));
}

static int16_t readS16_spi(uint8_t reg) {
    return (int16_t)read16_spi(reg);
}

// =====================================================
// Public API
// =====================================================

int8_t bme280_init_spi(uint16_t clk_div) {
    // Initialize hardware SPI (Mode 0 or 3 supported by Bosch)
    spi_init(clk_div, 0); 

    uint8_t id = bme280_read_reg_spi(BME280_CHIP_ID);
    if (id != 0x58 && id != 0x60) return -1;

    // Load Calibration parameters via SPI
    calib.dig_T1 = read16_spi(0x88);
    calib.dig_T2 = readS16_spi(0x8A);
    calib.dig_T3 = readS16_spi(0x8C);
    
    calib.dig_P1 = read16_spi(0x8E);
    calib.dig_P2 = readS16_spi(0x90);
    calib.dig_P3 = readS16_spi(0x92);
    calib.dig_P4 = readS16_spi(0x94);
    calib.dig_P5 = readS16_spi(0x96);
    calib.dig_P6 = readS16_spi(0x98);
    calib.dig_P7 = readS16_spi(0x9A);
    calib.dig_P8 = readS16_spi(0x9C);
    calib.dig_P9 = readS16_spi(0x9E);

    calib.dig_H1 = bme280_read_reg_spi(0xA1);
    calib.dig_H2 = readS16_spi(0xE1);
    calib.dig_H3 = bme280_read_reg_spi(0xE3);
    
    // Bit-packed Humidity calibration
    uint8_t e4 = bme280_read_reg_spi(0xE4);
    uint8_t e5 = bme280_read_reg_spi(0xE5);
    uint8_t e6 = bme280_read_reg_spi(0xE6);
    calib.dig_H4 = (int16_t)((e4 << 4) | (e5 & 0x0F));
    calib.dig_H5 = (int16_t)((e6 << 4) | (e5 >> 4));
    calib.dig_H6 = (int8_t)bme280_read_reg_spi(0xE7);

    // Default setup: Sleep mode, Oversampling x1
    bme280_write_reg_spi(BME280_REG_CTRL_HUM, 0x01);
    bme280_write_reg_spi(BME280_REG_CTRL_MEAS, 0x24); 
    
    return 0;
}

void bme280_get_temperature_spi(int32_t *temp) {
    // Trigger Forced Mode for a fresh reading
    bme280_write_reg_spi(BME280_REG_CTRL_MEAS, 0x25);
    while (bme280_read_reg_spi(0xF3) & 0x08); // Wait for busy bit

    uint8_t msb, lsb, xlsb;
    uint8_t addr = 0xFA | 0x80;

    spi_cs_low();
    spi_transfer(addr);
    msb  = spi_transfer(0x00);
    lsb  = spi_transfer(0x00);
    xlsb = spi_transfer(0x00);
    spi_cs_high();

    int32_t adc_T = (int32_t)(((uint32_t)msb << 12) | ((uint32_t)lsb << 4) | ((uint32_t)xlsb >> 4));

    int32_t v1 = ((((adc_T >> 3) - ((int32_t)calib.dig_T1 << 1))) * ((int32_t)calib.dig_T2)) >> 11;
    int32_t v2 = (((((adc_T >> 4) - ((int32_t)calib.dig_T1)) * ((adc_T >> 4) - ((int32_t)calib.dig_T1))) >> 12) * ((int32_t)calib.dig_T3)) >> 14;
    calib.t_fine = v1 + v2;
    *temp = (calib.t_fine * 5 + 128) >> 8;
  
}
void bme280_get_pressure_spi(uint32_t *press) {
    int32_t dummy; 
    bme280_get_temperature_spi(&dummy); // Updates calib.t_fine

    uint8_t msb, lsb, xlsb;
    uint8_t addr = 0xF7 | 0x80;

    spi_cs_low();
    spi_transfer(addr);
    msb  = spi_transfer(0x00); // 0xF7
    lsb  = spi_transfer(0x00); // 0xF8
    xlsb = spi_transfer(0x00); // 0xF9
    spi_cs_high();

    // 1. Fix ADC reconstruction (20-bit)
    int32_t adc_P = (int32_t)((((uint32_t)msb << 16) | ((uint32_t)lsb << 8) | (uint32_t)xlsb) >> 4);
    
    int64_t var1, var2, p;
    var1 = ((int64_t)calib.t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib.dig_P5) << 17);
    var2 = var2 + ((int64_t)calib.dig_P4 << 35);
    var1 = ((var1 * var1 * (int64_t)calib.dig_P3) >> 8) + ((var1 * (int64_t)calib.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib.dig_P1) >> 33;

    if (var1 == 0) {
        *press = 0;
        return;
    }

    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib.dig_P8) * p) >> 19;
    
    // Result is in Q24.8 format (Pa * 256)
    p = ((p + var1 + var2) >> 8) + ((int64_t)calib.dig_P7 << 4);

    // 2. Convert to hPa (Divide by 256 to get Pa, then 100 for hPa)
    // To keep it as an integer in hPa, use:
    *press = (uint32_t)(p / 256); 
}

void bme280_get_humidity_spi(uint32_t *hum) {
    int32_t dummy; bme280_get_temperature_spi(&dummy); 

    uint8_t msb, lsb;
    uint8_t addr = 0xFD | 0x80;

    spi_cs_low();
    spi_transfer(addr);
    msb = spi_transfer(0x00);
    lsb = spi_transfer(0x00);
    spi_cs_high();

    int32_t adc_H = (int32_t)(((uint32_t)msb << 8) | ((uint32_t)lsb));

    int32_t h = (calib.t_fine - ((int32_t)76800));
    h = (((((adc_H << 14) - (((int32_t)calib.dig_H4) << 20) - (((int32_t)calib.dig_H5) * h)) + 16384) >> 15) * (((((((h * ((int32_t)calib.dig_H6)) >> 10) * (((h * ((int32_t)calib.dig_H3)) >> 11) + 32768)) >> 10) + 2097152) * ((int32_t)calib.dig_H2) + 8192) >> 14));
    h = (h - (((((h >> 15) * (h >> 15)) >> 7) * ((int32_t)calib.dig_H1)) >> 4));
    h = (h < 0 ? 0 : (h > 419430400 ? 419430400 : h));
    *hum = (uint32_t)(h >> 12);
}