#ifndef SOFTI2C_H
#define SOFTI2C_H

#include <stdint.h>

/* ------------------------------
 * User configuration
 * ------------------------------ */

/* GPIO pin numbers */
#define SOFTI2C_SDA_PIN   0
#define SOFTI2C_SCL_PIN   1

/* ------------------------------
 * Public API
 * ------------------------------ */

/* Initialize I2C GPIOs (idle state) */
void softi2c_init(void);

/* I2C bus control */
void softi2c_start(void);
void softi2c_stop(void);

/* Byte-level operations */
uint8_t softi2c_write_byte(uint8_t data);   /* returns 1 = ACK, 0 = NACK */
uint8_t softi2c_read_byte(uint8_t ack);     /* ack = 1 send ACK, 0 send NACK */

/* Convenience helpers */
void softi2c_write_reg(uint8_t dev,
                       uint8_t reg,
                       uint8_t val);

uint8_t softi2c_read_reg(uint8_t dev,
                         uint8_t reg);

#endif /* SOFTI2C_H */
