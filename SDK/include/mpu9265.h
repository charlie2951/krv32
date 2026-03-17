#ifndef MPU9265_H
#define MPU9265_H

#include <stdint.h>

typedef enum {
    MPU_INTERFACE_SPI,
    MPU_INTERFACE_I2C
} mpu_interface_t;

typedef struct {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    int16_t mx, my, mz;
    int32_t temp_c; 
} MPU9265_Data;

// Function Prototypes
int MPU9265_Init(mpu_interface_t interface, void* i2c_inst);
void MPU9265_ReadAll(MPU9265_Data *data);

#endif