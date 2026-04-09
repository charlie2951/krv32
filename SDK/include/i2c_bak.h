#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// =====================================================
// Base Addresses
// =====================================================
#define I2C0_BASE_ADDR      0xC0000000UL
#define I2C1_BASE_ADDR      0xC0000100UL

// =====================================================
// I2C Instance Structure
// =====================================================
typedef struct
{
    uint32_t base_addr;
} i2c_t;

// =====================================================
// Register Offsets
// =====================================================
#define I2C_REG_CTRL       0x00
#define I2C_REG_DEV_ADDR   0x04
#define I2C_REG_REG_ADDR   0x08
#define I2C_REG_MOSI       0x0C
#define I2C_REG_MISO       0x10
#define I2C_REG_DIVIDER    0x14
#define I2C_REG_STATUS     0x18

// =====================================================
// Register Access Macro (instance based)
// =====================================================
#define I2C_REG(i2c, offset) \
    (*(volatile uint32_t *)((i2c)->base_addr + (offset)))

// =====================================================
// Control Bits
// =====================================================
#define I2C_CTRL_ENABLE      (1u << 0)
#define I2C_CTRL_READ_WRITE  (1u << 1)

// =====================================================
// Status Bits
// =====================================================
#define I2C_STATUS_BUSY      (1u << 0)

// =====================================================
// Public Instances
// =====================================================
extern i2c_t I2C0;
extern i2c_t I2C1;

// =====================================================
// API Prototypes
// =====================================================
void     i2c_init(i2c_t *i2c, uint16_t divider);
void     i2c_write(i2c_t *i2c, uint8_t device, uint8_t reg, uint8_t data);
uint8_t  i2c_read(i2c_t *i2c, uint8_t device, uint8_t reg);
void     i2c_wait_while_busy(i2c_t *i2c);


#ifdef __cplusplus
}
#endif

#endif
