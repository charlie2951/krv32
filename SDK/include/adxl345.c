#include "adxl345.h"
#define SPI_PORT SPI0
void adxl345_write_reg(uint8_t reg, uint8_t val) {
    spi_cs_low(SPI_PORT);
    spi_transfer(SPI_PORT,reg);    // Write: Bit 7 is 0
    spi_transfer(SPI_PORT,val);
    spi_cs_high(SPI_PORT);
}

uint8_t adxl345_read_reg(uint8_t reg) {
    uint8_t val;
    spi_cs_low(SPI_PORT);
    spi_transfer(SPI_PORT,reg | ADXL345_SPI_READ); // Read: Bit 7 is 1
    val = spi_transfer(SPI_PORT,0x00);          // Send dummy byte to clock in MISO
    spi_cs_high(SPI_PORT);
    return val;
}

/**
 * @brief Initialize ADXL345 using SPI Mode 3
 * @param clk_div Clock divider (e.g., 50 for 2MHz SPI if sys_clk is 100MHz)
 */
uint8_t adxl345_spi_init(uint16_t clk_div) {
    // 1. Initialize SPI Hardware in Mode 3 (CPOL=1, CPHA=1)
    spi_init(SPI_PORT,clk_div, 3);

    // 2. Simple power-on delay (approx 10ms depending on CPU speed)
    for(volatile int i = 0; i < 100000; i++);

    // 3. Verify Device ID (Should be 0xE5)
    uint8_t dev_id = adxl345_read_reg(ADXL345_REG_DEVID);
    if (dev_id != 0xE5) {
        return 0; // Device not found or communication error
    }

    // 4. Set Data Format: +/- 2g, Full Resolution, Right Justified
    // 0x08 = 0000 1000 (Full_Res=1, Range=00 for 2g)
    adxl345_write_reg(ADXL345_REG_DATA_FORMAT, 0x08);

    // 5. Enable Measurement Mode
    // 0x08 = 0000 1000 (Measure bit = 1)
    adxl345_write_reg(ADXL345_REG_POWER_CTL, 0x08);

    return 1; // Initialization successful
}

void adxl345_read_accel(adxl345_data_t* data) {
    uint8_t raw[6];

    spi_cs_low(SPI_PORT);
    // Read 6 bytes starting from DATAX0, set Multi-Byte (MB) bit
    spi_transfer(SPI_PORT,ADXL345_REG_DATAX0 | ADXL345_SPI_READ | ADXL345_SPI_MB);
    
    for(int i = 0; i < 6; i++) {
        raw[i] = spi_transfer(SPI_PORT,0x00); // Clock in data
    }
    spi_cs_high(SPI_PORT);

    // Reassemble 16-bit signed values (Little Endian: LSB, then MSB)
    data->x = (int16_t)((raw[1] << 8) | raw[0]);
    data->y = (int16_t)((raw[3] << 8) | raw[2]);
    data->z = (int16_t)((raw[5] << 8) | raw[4]);
}

int adxl345_i2c_init(i2c_t *i2c) {
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


void adxl345_i2c_read(i2c_t *i2c, adxl345_data_t *data) {
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