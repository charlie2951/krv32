#include "i2c.h"

// =====================================================
// Instance Definitions
// =====================================================
i2c_t I2C0 = { I2C0_BASE_ADDR };
i2c_t I2C1 = { I2C1_BASE_ADDR };

// =====================================================
// Wait until controller is idle
// =====================================================
void i2c_wait_while_busy(i2c_t *i2c)
{
    while (I2C_REG(i2c, I2C_REG_STATUS) & I2C_STATUS_BUSY);
}

// =====================================================
// Initialize I2C clock divider
// divider = (system_clk / (2 * i2c_clk)) - 1
// =====================================================
void i2c_init(i2c_t *i2c, uint16_t divider)
{
    I2C_REG(i2c, I2C_REG_DIVIDER) = divider;
}

// =====================================================
// Blocking Write Transaction
// =====================================================
void i2c_write(i2c_t *i2c, uint8_t device, uint8_t reg, uint8_t data)
{
    I2C_REG(i2c, I2C_REG_DEV_ADDR) = device;
    I2C_REG(i2c, I2C_REG_REG_ADDR) = reg;
    I2C_REG(i2c, I2C_REG_MOSI)     = data;

    // Write mode + enable
    I2C_REG(i2c, I2C_REG_CTRL) = I2C_CTRL_ENABLE;

    i2c_wait_while_busy(i2c);
}


// =====================================================
// Blocking Read Transaction
// =====================================================
uint8_t i2c_read(i2c_t *i2c, uint8_t device, uint8_t reg)
{
    uint8_t value;

    I2C_REG(i2c, I2C_REG_DEV_ADDR) = device;
    I2C_REG(i2c, I2C_REG_REG_ADDR) = reg;

    // Read mode + enable
    I2C_REG(i2c, I2C_REG_CTRL) =
        I2C_CTRL_ENABLE | I2C_CTRL_READ_WRITE;

    i2c_wait_while_busy(i2c);

    value = (uint8_t)(I2C_REG(i2c, I2C_REG_MISO) & 0xFF);

    return value;
}
