#include "i2c_adxl345.h"

int adxl345_init(i2c_t *i2c) {
    // 1. Verify connection
    uint8_t id = i2c_read(i2c, ADXL345_ADDR, ADXL345_REG_DEVID);
    if (id != ADXL345_DEVICE_ID) {
        return -1; 
    }

    // 2. Data Format: Full Resolution, +/- 16g Range
    // 0x08 (Full Res) | 0x03 (+/- 16g) = 0x0B
    i2c_write(i2c, ADXL345_ADDR, ADXL345_REG_DATA_FORMAT, 0x0B);

    // 3. Power Control: Start Measurement
    // Setting bit 3 (Measure) to 1
    i2c_write(i2c, ADXL345_ADDR, ADXL345_REG_POWER_CTL, 0x08);

    return 0;
}

void adxl345_read(i2c_t *i2c, adxl345_data_t *data) {
    // Reading 6 bytes sequentially starting from DATAX0
    // Register 0x32: X-Low, 0x33: X-High
    // Register 0x34: Y-Low, 0x35: Y-High
    // Register 0x36: Z-Low, 0x37: Z-High

    uint8_t x0 = i2c_read(i2c, ADXL345_ADDR, ADXL345_REG_DATAX0);
    uint8_t x1 = i2c_read(i2c, ADXL345_ADDR, ADXL345_REG_DATAX0 + 1);
    data->x = (int16_t)((x1 << 8) | x0);

    uint8_t y0 = i2c_read(i2c, ADXL345_ADDR, ADXL345_REG_DATAX0 + 2);
    uint8_t y1 = i2c_read(i2c, ADXL345_ADDR, ADXL345_REG_DATAX0 + 3);
    data->y = (int16_t)((y1 << 8) | y0);

    uint8_t z0 = i2c_read(i2c, ADXL345_ADDR, ADXL345_REG_DATAX0 + 4);
    uint8_t z1 = i2c_read(i2c, ADXL345_ADDR, ADXL345_REG_DATAX0 + 5);
    data->z = (int16_t)((z1 << 8) | z0);
}