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
#include "softi2c.h"

/* -------------------------------------------------
 * Low-level register access
 * ------------------------------------------------- */

void mpu6050_write_reg(uint8_t reg, uint8_t val)
{
    softi2c_start();
    softi2c_write_byte(MPU6050_I2C_ADDR << 1);   // write
    softi2c_write_byte(reg);
    softi2c_write_byte(val);
    softi2c_stop();
}

//revised burst read
static void mpu6050_read(uint8_t reg, uint8_t *buf, uint8_t len)
{
    if (len == 0) return;

    // Write register address //
    softi2c_start();
    softi2c_write_byte(MPU6050_I2C_ADDR << 1);   // WRITE
    softi2c_write_byte(reg);

    // Repeated START for read //
    softi2c_start();
    softi2c_write_byte((MPU6050_I2C_ADDR << 1) | 1); // READ

    // Read bytes //
    for (uint8_t i = 0; i < len; i++)
        buf[i] = softi2c_read_byte(i < (len - 1)); // ACK all but last

    softi2c_stop();
}


uint8_t mpu6050_read_reg(uint8_t reg)
{
    uint8_t val;

    val = softi2c_read_reg(MPU6050_I2C_ADDR, reg);
    return val;
}

// -------------------------------------------------
 // Device control
 // ------------------------------------------------- 

uint8_t mpu6050_who_am_i(void)
{
    return mpu6050_read_reg(MPU6050_REG_WHO_AM_I);
}

uint8_t mpu6050_init(void)
{
    // Wake up device (clear sleep bit) //
    mpu6050_write_reg(MPU6050_REG_PWR_MGMT_1, 0x00);

    // Small delay after wake-up (simple busy wait) //
    for (volatile int i = 0; i < 10000; i++);

    /* Sample rate = Gyro rate / (1 + SMPLRT_DIV)
       Gyro rate default = 8kHz
       SMPLRT_DIV = 79 → 100 Hz */
    mpu6050_write_reg(MPU6050_REG_SMPLRT_DIV, 79);

    /* DLPF config: ~44 Hz */
    mpu6050_write_reg(MPU6050_REG_CONFIG, 0x03);

    /* Gyro full scale = ±250 dps */
    mpu6050_write_reg(MPU6050_REG_GYRO_CONFIG, 0x00);

    /* Accel full scale = ±2g */
    mpu6050_write_reg(MPU6050_REG_ACCEL_CONFIG, 0x00);

    /* Verify device ID */
    return (mpu6050_who_am_i() == MPU6050_I2C_ADDR);
}

/* -------------------------------------------------
 * Sensor reads
 * ------------------------------------------------- */

void mpu6050_read_accel( int16_t *ax,
                         int16_t *ay,
                         int16_t *az)
{
    uint8_t hi, lo;

      hi = mpu6050_read_reg(MPU6050_REG_ACCEL_XOUT_H);
     lo = mpu6050_read_reg(MPU6050_REG_ACCEL_XOUT_H + 1);
     *ax = (int16_t)((hi << 8) | lo);

    hi = mpu6050_read_reg(MPU6050_REG_ACCEL_XOUT_H + 2);
    lo = mpu6050_read_reg(MPU6050_REG_ACCEL_XOUT_H + 3);
    *ay = (int16_t)((hi << 8) | lo);

    hi = mpu6050_read_reg(MPU6050_REG_ACCEL_XOUT_H + 4);
    lo = mpu6050_read_reg(MPU6050_REG_ACCEL_XOUT_H + 5);
    *az = (int16_t)((hi << 8) | lo);
}
    
/*
//revised code to read accl
void mpu6050_read_accel(int16_t *ax,
                        int16_t *ay,
                        int16_t *az)
{
    uint8_t buf[6];

    mpu6050_read(MPU6050_REG_ACCEL_XOUT_H, buf, 6);

    *ax = ((int16_t)buf[0] << 8) | buf[1];
    *ay = ((int16_t)buf[2] << 8) | buf[3];
    *az = ((int16_t)buf[4] << 8) | buf[5];
}
*/

void mpu6050_read_gyro(int16_t *gx,
                       int16_t *gy,
                       int16_t *gz)
{
    uint8_t hi, lo;

    hi = mpu6050_read_reg(MPU6050_REG_GYRO_XOUT_H);
    lo = mpu6050_read_reg(MPU6050_REG_GYRO_XOUT_H + 1);
    *gx = (int16_t)((hi << 8) | lo);

    hi = mpu6050_read_reg(MPU6050_REG_GYRO_XOUT_H + 2);
    lo = mpu6050_read_reg(MPU6050_REG_GYRO_XOUT_H + 3);
    *gy = (int16_t)((hi << 8) | lo);

    hi = mpu6050_read_reg(MPU6050_REG_GYRO_XOUT_H + 4);
    lo = mpu6050_read_reg(MPU6050_REG_GYRO_XOUT_H + 5);
    *gz = (int16_t)((hi << 8) | lo);
}

/*
//revised gyro read
void mpu6050_read_gyro(int16_t *gx,
                       int16_t *gy,
                       int16_t *gz)
{
    uint8_t buf[6];

    mpu6050_read(MPU6050_REG_GYRO_XOUT_H, buf, 6);

    *gx = ((int16_t)buf[0] << 8) | buf[1];
    *gy = ((int16_t)buf[2] << 8) | buf[3];
    *gz = ((int16_t)buf[4] << 8) | buf[5];
}
*/

int16_t mpu6050_read_temp(void)
{
    uint8_t hi, lo;

    hi = mpu6050_read_reg(MPU6050_REG_TEMP_OUT_H);
    lo = mpu6050_read_reg(MPU6050_REG_TEMP_OUT_H + 1);

    return (int16_t)((hi << 8) | lo);
}

/*
//revised temp read
int16_t mpu6050_read_temp(void)
{
    uint8_t buf[2];

    mpu6050_read(MPU6050_REG_TEMP_OUT_H, buf, 2);

    return ((int16_t)buf[0] << 8) | buf[1];
}
    */


