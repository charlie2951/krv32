/*
MIT License

Copyright (c) 2024-2026 Subir Maity

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "mpu6050.h"

// Small delay loop
static void delay(void)
{
    for (volatile int i = 0; i < 10000; i++);
}

// ========================================================
// Initialize MPU6050
// ========================================================
void mpu_init(i2c_t *i2c)
{
    // divider = (system_clk / (2 * i2c_clk)) - 1
    // Example: 100MHz → 100kHz → divider ≈ 499
    i2c_init(i2c, 499);  // 100KHz
    // i2c_init(i2c, 124); // 400KHz

    delay();

    // Wake up device
    i2c_write(i2c, MPU6050_ADDR, MPU_PWR_MGMT_1, 0x00);

    delay();
}

// ========================================================
// WHO_AM_I
// ========================================================
uint8_t mpu_whoami(i2c_t *i2c)
{
    return i2c_read(i2c, MPU6050_ADDR, MPU_WHO_AM_I);
}

// ========================================================
// Accelerometer
// ========================================================
void mpu_read_accel(i2c_t *i2c,
                    int16_t *ax,
                    int16_t *ay,
                    int16_t *az)
{
    uint8_t hx, lx;

    hx = i2c_read(i2c, MPU6050_ADDR, MPU_ACCEL_XOUT_H);
    lx = i2c_read(i2c, MPU6050_ADDR, MPU_ACCEL_XOUT_H + 1);
    *ax = (int16_t)((hx << 8) | lx);

    hx = i2c_read(i2c, MPU6050_ADDR, MPU_ACCEL_XOUT_H + 2);
    lx = i2c_read(i2c, MPU6050_ADDR, MPU_ACCEL_XOUT_H + 3);
    *ay = (int16_t)((hx << 8) | lx);

    hx = i2c_read(i2c, MPU6050_ADDR, MPU_ACCEL_XOUT_H + 4);
    lx = i2c_read(i2c, MPU6050_ADDR, MPU_ACCEL_XOUT_H + 5);
    *az = (int16_t)((hx << 8) | lx);
}

// ========================================================
// Gyroscope
// ========================================================
void mpu_read_gyro(i2c_t *i2c,
                   int16_t *gx,
                   int16_t *gy,
                   int16_t *gz)
{
    uint8_t hx, lx;

    hx = i2c_read(i2c, MPU6050_ADDR, MPU_GYRO_XOUT_H);
    lx = i2c_read(i2c, MPU6050_ADDR, MPU_GYRO_XOUT_H + 1);
    *gx = (int16_t)((hx << 8) | lx);

    hx = i2c_read(i2c, MPU6050_ADDR, MPU_GYRO_XOUT_H + 2);
    lx = i2c_read(i2c, MPU6050_ADDR, MPU_GYRO_XOUT_H + 3);
    *gy = (int16_t)((hx << 8) | lx);

    hx = i2c_read(i2c, MPU6050_ADDR, MPU_GYRO_XOUT_H + 4);
    lx = i2c_read(i2c, MPU6050_ADDR, MPU_GYRO_XOUT_H + 5);
    *gz = (int16_t)((hx << 8) | lx);
}

// ========================================================
// Temperature
// ========================================================
int16_t mpu_read_temp(i2c_t *i2c)
{
    uint8_t hi, lo;

    hi = i2c_read(i2c, MPU6050_ADDR, MPU_TEMP_OUT_H);
    lo = i2c_read(i2c, MPU6050_ADDR, MPU_TEMP_OUT_H + 1);

    return (int16_t)((hi << 8) | lo);
}


