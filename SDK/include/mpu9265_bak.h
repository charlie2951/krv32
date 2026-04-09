#ifndef MPU9265_H
#define MPU9265_H

#include <stdint.h>

#define MPU9265_READ_BIT      0x80

// MPU9265 Registers
#define REG_USER_CTRL         0x6A
#define REG_I2C_MST_CTRL      0x24
#define REG_I2C_SLV0_ADDR     0x25
#define REG_I2C_SLV0_REG      0x26
#define REG_I2C_SLV0_CTRL     0x27
#define REG_EXT_SENS_DATA_00  0x49

// AK8963 Magnetometer Registers (Internal I2C address 0x0C)
#define AK8963_I2C_ADDR       0x0C
#define AK8963_CNTL1          0x0A
#define AK8963_HXL            0x03 // Mag data starts here

typedef struct {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    int16_t mx, my, mz;
    int16_t temp_c;
} MPU9265_Data;

int MPU9265_Init(void);
void MPU9265_ReadAll(MPU9265_Data *data);
#endif