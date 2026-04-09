#include "soft_ads1115.h"
#include "soft_i2c.c"
/**
 * @brief Writes 16-bit data to ADS1115 using Software I2C
 */
void soft_ads1115_write_reg16(uint8_t reg, uint16_t data) {
    soft_i2c_start();
    soft_i2c_write_byte(ADS1115_ADDR << 1); 
    soft_i2c_write_byte(reg);               
    soft_i2c_write_byte((uint8_t)(data >> 8));   
    soft_i2c_write_byte((uint8_t)(data & 0xFF)); 
    soft_i2c_stop();
}

/**
 * @brief Reads 16-bit data from ADS1115 using Software I2C
 */
uint16_t soft_ads1115_read_reg16(uint8_t reg) {
    uint16_t val;
    
    // Step 1: Tell the ADS1115 which register we want to read
    soft_i2c_start();
    soft_i2c_write_byte(ADS1115_ADDR << 1);
    soft_i2c_write_byte(reg);
    
    // Step 2: Repeated Start to switch to Read mode
    soft_i2c_start(); 
    soft_i2c_write_byte((ADS1115_ADDR << 1) | 1); 
    
    // Step 3: Read MSB then LSB
    val = (uint16_t)soft_i2c_read_byte(1) << 8; // Master ACKs the first byte
    val |= (uint16_t)soft_i2c_read_byte(0);    // Master NACKs the last byte
    soft_i2c_stop();
    
    return val;
}

/**
 * @brief Helper to get Millivolts
 */
int32_t soft_ads1115_get_mv() {
    uint16_t raw_u = soft_ads1115_read_reg16(ADS_REG_CONV);
    int16_t raw = (int16_t)raw_u;
    return ((int32_t)raw * 125) / 1000;
}