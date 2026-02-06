#include "softi2c.h"
#include "delay.h"
/* GPIO registers */
#define GPIO_DATA (*(volatile uint32_t *)0x10000000)
#define GPIO_DIR  (*(volatile uint32_t *)0x10000004)

/* ------------------------------
 * Low-level helpers (open-drain)
 * ------------------------------ */

static inline void sda_low(void)
{
    GPIO_DIR  |=  (0x00000001 << SOFTI2C_SDA_PIN);
    GPIO_DATA &= ~(0x00000001 << SOFTI2C_SDA_PIN);
}

static inline void sda_release(void)
{
    GPIO_DIR &= ~(0x00000001 << SOFTI2C_SDA_PIN);
}

static inline void scl_low(void)
{
    GPIO_DIR  |=  (0x00000001 << SOFTI2C_SCL_PIN);
    GPIO_DATA &= ~(0x00000001 << SOFTI2C_SCL_PIN);
}

static inline void scl_release(void)
{
    GPIO_DIR &= ~(0x00000001 << SOFTI2C_SCL_PIN);
}

static inline uint8_t sda_read(void)
{
    return (GPIO_DATA >> SOFTI2C_SDA_PIN) & 0x00000001;
}

/* ------------------------------
 * Timing
 * ------------------------------ */

static void i2c_delay(void)
{
    delay(1000);//set max value on trial and error
}

/* ------------------------------
 * Public functions
 * ------------------------------ */

void softi2c_init(void)
{
    sda_release();
    scl_release();
}

void softi2c_start(void)
{
    sda_release();
    scl_release();
    i2c_delay();

    sda_low();
    i2c_delay();

    scl_low();
}

void softi2c_stop(void)
{
    sda_low();
    i2c_delay();

    scl_release();
    i2c_delay();

    sda_release();
    i2c_delay();
}

static void softi2c_write_bit(uint8_t bit)
{
    if (bit)
        sda_release();
    else
        sda_low();

    i2c_delay();
    scl_release();
    i2c_delay();
    scl_low();
}

static uint8_t softi2c_read_bit(void)
{
    uint8_t bit;

    sda_release();
    i2c_delay();

    scl_release();
    i2c_delay();
    i2c_delay();
    bit = sda_read();
    scl_low();
    i2c_delay();
    return bit;
}
/*
uint8_t softi2c_write_byte(uint8_t data)
{
    for (int i = 7; i >= 0; i--)
        softi2c_write_bit((data >> i) & 0x00000001);

    // ACK = 0 //
    return (softi2c_read_bit() == 0);
}
*/
uint8_t softi2c_write_byte(uint8_t data)
{
    for (int i = 7; i >= 0; i--)
        softi2c_write_bit((data >> i) & 0x01);

    /* -------- ACK cycle -------- */
    sda_release();        // SDA = input
    i2c_delay();

    scl_release();        // SCL high
    i2c_delay();
    i2c_delay();          // 🔥 give slave time

    uint8_t ack = sda_read();

    scl_low();
    i2c_delay();
    /* --------------------------- */

    return (ack == 0);    // ACK is active LOW
}

uint8_t softi2c_read_byte(uint8_t ack)
{
    uint8_t data = 0;

    for (int i = 7; i >= 0; i--)
    {
        data <<= 1;
        data |= softi2c_read_bit();
    }

    /* ACK=0, NACK=1 */
    softi2c_write_bit(!ack);

    return data;
}

void softi2c_write_reg(uint8_t dev,
                       uint8_t reg,
                       uint8_t val)
{
    softi2c_start();
    softi2c_write_byte(dev << 1);
    softi2c_write_byte(reg);
    softi2c_write_byte(val);
    softi2c_stop();
}

uint8_t softi2c_read_reg(uint8_t dev,
                         uint8_t reg)
{
    uint8_t val;

    softi2c_start();
    softi2c_write_byte(dev << 1);
    softi2c_write_byte(reg);

    softi2c_start();
    softi2c_write_byte((dev << 1) | 1);
    val = softi2c_read_byte(0);   // NACK
    softi2c_stop();

    return val;
}
