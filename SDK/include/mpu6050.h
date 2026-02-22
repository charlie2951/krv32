#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include "i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

// ========================================================
// MPU6050 I2C Address
// ========================================================
#define MPU6050_ADDR        0x68

// ========================================================
// Register Map
// ========================================================
#define MPU_PWR_MGMT_1      0x6B
#define MPU_WHO_AM_I        0x75

#define MPU_ACCEL_XOUT_H    0x3B
#define MPU_GYRO_XOUT_H     0x43
#define MPU_TEMP_OUT_H      0x41

// ========================================================
// API
// ========================================================
void     mpu_init(i2c_t *i2c);
uint8_t  mpu_whoami(i2c_t *i2c);

void     mpu_read_accel(i2c_t *i2c,
                        int16_t *ax,
                        int16_t *ay,
                        int16_t *az);

void     mpu_read_gyro(i2c_t *i2c,
                       int16_t *gx,
                       int16_t *gy,
                       int16_t *gz);

int16_t  mpu_read_temp(i2c_t *i2c);

#ifdef __cplusplus
}
#endif

#endif
