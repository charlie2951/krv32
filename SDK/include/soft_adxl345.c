#include "soft_adxl345.h"

void soft_adxl_write_reg(uint8_t reg, uint8_t val) {
    spi_bb_cs_low();
    // For Write: Bit 7 is 0.
    spi_bb_transfer(reg & 0x7F); 
    spi_bb_transfer(val);
    spi_bb_cs_high();
}

uint8_t soft_adxl_read_reg(uint8_t reg) {
    uint8_t val;
    spi_bb_cs_low();
    // For Read: Bit 7 is 1.
    spi_bb_transfer(reg | ADXL_READ_BIT);
    val = spi_bb_transfer(0xFF);
    spi_bb_cs_high();
    return val;
}
/*
uint8_t soft_adxl_init(void) {
    // 1. Hardware setup
    spi_bb_init();
    
    // 2. Short delay for sensor power-on stabilization
    for(volatile int i = 0; i < 100000; i++);

    // 3. Check ID
    uint8_t id = soft_adxl_read_reg(ADXL_REG_DEVID);
    if (id != 0xE5) {
        return id; // Return the wrong ID for debugging
    }

    // 4. Data Format: +/- 2g, Full Res (0x08)
    soft_adxl_write_reg(ADXL_REG_DATA_FORMAT, 0x08);

    // 5. Power Control: Measure Mode (0x08)
    soft_adxl_write_reg(ADXL_REG_POWER_CTL, 0x08);

    return 0xE5; // Success
}
*/

uint8_t soft_adxl_init(void) {
    for(volatile int i=0; i<1000; i++);//starting delay
    spi_bb_init();
    // Force SPI Mode: Toggle CS high and low 
    // This can help "reset" the chip's interface logic
   // spi_bb_cs_high();
   // for(volatile int i=0; i<1000; i++);
   //set CS to low as per adxl spec
   //  spi_bb_cs_low();
  //  for(volatile int i=0; i<1000; i++);
   // spi_bb_cs_high();
   // for(volatile int i=0; i<100000; i++); // Wait for it to settle

    // Try reading ID
    uint8_t id = soft_adxl_read_reg(ADXL_REG_DEVID);
    
    if (id == 0xE5) {
        soft_adxl_write_reg(ADXL_REG_DATA_FORMAT, 0x08); // +/- 2g, full res
        soft_adxl_write_reg(ADXL_REG_POWER_CTL, 0x08);   // Measurement mode
        return 0xE5;
    }
    
    return id; // Return the 0 or whatever was received
}

void soft_adxl_read_accel(soft_adxl_data_t* data) {
    uint8_t raw[6];
    
    spi_bb_cs_low();
    // Start reading at DATAX0, set Read and Multi-Byte bits
    spi_bb_transfer(ADXL_REG_DATAX0 | ADXL_READ_BIT | ADXL_MB_BIT);
    
    for(int i = 0; i < 6; i++) {
        raw[i] = spi_bb_transfer(0xFF);
    }
    spi_bb_cs_high();

    // Mapping: X0, X1, Y0, Y1, Z0, Z1
    data->x = (int16_t)((raw[1] << 8) | raw[0]);
    data->y = (int16_t)((raw[3] << 8) | raw[2]);
    data->z = (int16_t)((raw[5] << 8) | raw[4]);
}