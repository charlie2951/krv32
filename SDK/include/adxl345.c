#include "adxl345.h"

void adxl345_write_reg(uint8_t reg, uint8_t val) {
    spi_cs_low();
    spi_transfer(reg);    // Write: Bit 7 is 0
    spi_transfer(val);
    spi_cs_high();
}

uint8_t adxl345_read_reg(uint8_t reg) {
    uint8_t val;
    spi_cs_low();
    spi_transfer(reg | ADXL345_SPI_READ); // Read: Bit 7 is 1
    val = spi_transfer(0x00);          // Send dummy byte to clock in MISO
    spi_cs_high();
    return val;
}

/**
 * @brief Initialize ADXL345 using SPI Mode 3
 * @param clk_div Clock divider (e.g., 50 for 2MHz SPI if sys_clk is 100MHz)
 */
uint8_t adxl345_spi_init(uint16_t clk_div) {
    // 1. Initialize SPI Hardware in Mode 3 (CPOL=1, CPHA=1)
    spi_init(clk_div, 3);

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

    spi_cs_low();
    // Read 6 bytes starting from DATAX0, set Multi-Byte (MB) bit
    spi_transfer(ADXL345_REG_DATAX0 | ADXL345_SPI_READ | ADXL345_SPI_MB);
    
    for(int i = 0; i < 6; i++) {
        raw[i] = spi_transfer(0x00); // Clock in data
    }
    spi_cs_high();

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
}

