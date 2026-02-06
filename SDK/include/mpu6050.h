#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

/* -------------------------------------------------
 * I2C address
 * ------------------------------------------------- */
/* AD0 = 0 → 0x68, AD0 = 1 → 0x69 */
#define MPU6050_I2C_ADDR      0x68

/* -------------------------------------------------
 * MPU6050 Register Map (partial)
 * ------------------------------------------------- */
#define MPU6050_REG_SMPLRT_DIV    0x19
#define MPU6050_REG_CONFIG        0x1A
#define MPU6050_REG_GYRO_CONFIG   0x1B
#define MPU6050_REG_ACCEL_CONFIG  0x1C
#define MPU6050_REG_ACCEL_XOUT_H  0x3B
#define MPU6050_REG_TEMP_OUT_H    0x41
#define MPU6050_REG_GYRO_XOUT_H   0x43
#define MPU6050_REG_PWR_MGMT_1    0x6B
#define MPU6050_REG_WHO_AM_I      0x75

/* -------------------------------------------------
 * Public API
 * ------------------------------------------------- */

/* Basic control */
uint8_t mpu6050_init(void);
uint8_t mpu6050_who_am_i(void);

/* Raw sensor reads */
void mpu6050_read_accel( int16_t *ax,
                         int16_t *ay,
                         int16_t *az);

void mpu6050_read_gyro(int16_t *gx,
                       int16_t *gy,
                       int16_t *gz);

int16_t mpu6050_read_temp(void);

/* Low-level access (optional) */
void    mpu6050_write_reg(uint8_t reg, uint8_t val);
uint8_t mpu6050_read_reg(uint8_t reg);
static void mpu6050_read(uint8_t reg, uint8_t *buf, uint8_t len);

#endif /* MPU6050_H */
