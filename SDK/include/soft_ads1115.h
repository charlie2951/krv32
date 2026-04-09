#ifndef SOFT_ADS1115_H
#define SOFT_ADS1115_H

#include <stdint.h>

// =====================================================
// ADS1115 I2C Address (ADDR pin to GND)
// =====================================================
#define ADS1115_ADDR          0x48

// =====================================================
// ADS1115 Register Pointers
// =====================================================
#define ADS_REG_CONV          0x00
#define ADS_REG_CONFIG        0x01
#define ADS_REG_LO_THRESH     0x02
#define ADS_REG_HI_THRESH     0x03

// =====================================================
// Config Register Bit Definitions
// =====================================================
#define ADS_OS_START          (1 << 15)
#define ADS_MUX_SINGLE_A0     (4 << 12) // AIN0 vs GND
#define ADS_MUX_SINGLE_A1     (5 << 12) // AIN1 vs GND
#define ADS_MUX_SINGLE_A2     (6 << 12) // AIN2 vs GND
#define ADS_MUX_SINGLE_A3     (7 << 12) // AIN3 vs GND
#define ADS_PGA_4096          (1 << 9)  // +/-4.096V range
#define ADS_MODE_SINGLE       (1 << 8)  // Single-shot mode
#define ADS_DR_128SPS         (4 << 5)  // 128 samples per second
#define ADS_COMP_QUE_OFF      (3 << 0)  // Disable comparator

// =====================================================
// Low-Level Software I2C Functions
// =====================================================


// =====================================================
// ADS1115 High-Level Functions
// =====================================================

/**
 * @brief Writes a 16-bit word to an ADS1115 register.
 */
void soft_ads1115_write_reg16(uint8_t reg, uint16_t data);

/**
 * @brief Reads a 16-bit word from an ADS1115 register using Repeated Start.
 */
uint16_t soft_ads1115_read_reg16(uint8_t reg);

/**
 * @brief Requests a single conversion on a specific channel (0-3).
 */
void soft_ads1115_request_read(uint8_t channel);

/**
 * @brief Reads the conversion register and returns the value in millivolts.
 * Uses 1 bit = 125uV (for the 4.096V PGA range).
 */
int32_t soft_ads1115_get_mv(void);

/**
 * @brief Checks if the ADS1115 is responding with the correct default config.
 */
int soft_ads1115_check_presence(void);

#endif // SOFT_ADS1115_H