#include "ads1115.h"
#include "i2c.h"

/**
 * @brief Writes a 16-bit word to the ADS1115 using two 8-bit transactions.
 * Note: This sends a STOP condition between bytes.
 */
void ads1115_write_reg16(i2c_t *i2c, uint8_t reg, uint16_t data) {
    uint8_t msb = (uint8_t)(data >> 8);
    uint8_t lsb = (uint8_t)(data & 0xFF);

    // Send MSB
    i2c_write(i2c, ADS1115_ADDR, reg, msb);
    
    // Send LSB 
    // Note: We don't increment 'reg' because the ADS1115 uses 
    // one address pointer for a 16-bit register pair.
    i2c_write(i2c, ADS1115_ADDR, reg, lsb);
}

/**
 * @brief Reads a 16-bit word from the ADS1115 using two 8-bit transactions.
 */
uint16_t ads1115_read_reg16(i2c_t *i2c, uint8_t reg) {
    uint8_t msb = i2c_read(i2c, ADS1115_ADDR, reg);
    uint8_t lsb = i2c_read(i2c, ADS1115_ADDR, reg);

    return (uint16_t)((msb << 8) | lsb);
}

/**
 * @brief Triggers a single-shot conversion on a specific channel.
 * Config: OS(1), PGA(4.096V), Mode(Single-Shot), DR(128SPS)
 */
void ads1115_request_read(i2c_t *i2c, uint8_t channel) {
    // Base config: 0x8183 (1000 0001 1000 0011)
    uint16_t config = 0x8183; 
    
    // Set MUX bits (bits 14:12)
    config |= ((uint16_t)(channel & 0x07) << 12); 

    ads1115_write_reg16(i2c, ADS_REG_CONFIG, config);
}

/**
 * @brief Reads the conversion result and converts to millivolts.
 */
int32_t ads1115_get_milli_volts(i2c_t *i2c) {
    uint16_t raw_u = ads1115_read_reg16(i2c, ADS_REG_CONV);
    int16_t raw = (int16_t)raw_u; // Cast to signed for negative diffs

    // PGA 4.096V range: 1 bit = 125 microvolts
    // (raw * 125) / 1000 gives millivolts
    return ((int32_t)raw * 125) / 1000;
}

/**
 * @brief Validates communication with the sensor.
 */
int ads1115_check_presence(i2c_t *i2c) {
    uint16_t config = ads1115_read_reg16(i2c, ADS_REG_CONFIG);

    // Default power-up config is 0x8583
    if ((config & 0xFF80) == 0x8580) { // Masking low bits as they may change
        return 1; 
    }
    return 0;
}