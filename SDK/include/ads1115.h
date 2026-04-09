#ifndef ADS1115_H
#define ADS1115_H

#include <stdint.h>
#include "soft_i2c.c"

// =====================================================
// ADS1115 I2C Address (ADDR pin to GND)
// =====================================================
#define ADS1115_ADDR          0x48

// =====================================================
// ADS1115 Register Pointers
// =====================================================
#define ADS_REG_CONV          0x00  // Conversion Register (Read Only)
#define ADS_REG_CONFIG        0x01  // Config Register (Read/Write)
#define ADS_REG_LO_THRESH     0x02  // Low Threshold (Read/Write)
#define ADS_REG_HI_THRESH     0x03  // High Threshold (Read/Write)

// =====================================================
// Config Register Bit Definitions (Defaults)
// =====================================================
// OS[15], MUX[14:12], PGA[11:9], MODE[8], DR[7:5], COMP_MODE[4], etc.
#define ADS_CONFIG_OS_SINGLE  0x8000  // Start a single conversion
#define ADS_CONFIG_MUX_A0     0x4000  // Single-ended AIN0
#define ADS_CONFIG_PGA_4096   0x0200  // +/-4.096V range (1 bit = 125uV)
#define ADS_CONFIG_MODE_SINGLE 0x0100 // Single-shot mode
#define ADS_CONFIG_DR_128SPS  0x0080  // 128 samples per second
#define ADS_CONFIG_COMP_QUE_OFF 0x0003 // Disable comparator

// =====================================================
// Function Prototypes
// =====================================================

/**
 * @brief Initialize the ADS1115 presence check
 * @return 1 if found, 0 if failed
 */
int ads1115_check_presence(i2c_t *i2c);

/**
 * @brief Request a conversion on a specific channel (0-3)
 */
void ads1115_request_read(i2c_t *i2c, uint8_t channel);

/**
 * @brief Reads the 16-bit raw value from the conversion register
 * Works by calling i2c_read twice (8-bit hardware limitation)
 */
uint16_t ads1115_read_reg16(i2c_t *i2c, uint8_t reg);

/**
 * @brief Writes a 16-bit value to a register
 * Works by calling i2c_write twice (8-bit hardware limitation)
 */
void ads1115_write_reg16(i2c_t *i2c, uint8_t reg, uint16_t data);

/**
 * @brief Full helper to get the voltage in millivolts
 */
int32_t ads1115_get_milli_volts(i2c_t *i2c);

#endif // ADS1115_H