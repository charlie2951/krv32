#include "mpu9265.h"
#include "spi.h"

// Internal Helper: Write a register
static void mpu9265_write_reg(uint8_t reg, uint8_t val) {
    spi_cs_low();
    spi_transfer(reg);
    spi_transfer(val);
    spi_cs_high();
}

// Internal Helper: Read a register
static uint8_t mpu9265_read_reg(uint8_t reg) {
    uint8_t val;
    spi_cs_low();
    spi_transfer(reg | MPU9265_READ_BIT);
    val = spi_transfer(0x00);
    spi_cs_high();
    return val;
}
/*
int MPU9265_Init(void) {
    spi_init(200,3);
    spi_cs_high();
    for(volatile int i=0;i<1000000; i++);//small delay-1sec to initialize sensor
    spi_cs_low();

    // Verify WHO_AM_I (0x71 is typical for this family)
    uint8_t id = mpu9265_read_reg(MPU9265_REG_WHO_AM_I);
    if (id != 0x71 && id != 0x73) {
        return -1; 
    }

    // Reset Device: Set H_RESET bit
    mpu9265_write_reg(MPU9265_REG_PWR_MGMT_1, 0x80);
    for(volatile int i = 0; i < 5000; i++); 

    // Wake up & set clock source to Gyro Z (more stable than internal RC)
    mpu9265_write_reg(MPU9265_REG_PWR_MGMT_1, 0x01);

    // Set Accel Range to +/- 2g (Register 0x1C, bits [4:3] = 00)
    mpu9265_write_reg(MPU9265_REG_ACCEL_CONFIG, 0x00);

    // Set Gyro Range to +/- 250 dps (Register 0x1B, bits [4:3] = 00)
    mpu9265_write_reg(MPU9265_REG_GYRO_CONFIG, 0x00);

    return 0;
}
*/
/*
void MPU9265_Read(MPU9265_Data *data) {
    uint8_t raw[14];
    
    // Burst read 14 bytes starting from Accel X High
    spi_cs_low();
    spi_transfer(MPU9265_REG_ACCEL_XOUT_H | MPU9265_READ_BIT);
    for (int i = 0; i < 14; i++) {
        raw[i] = spi_transfer(0x00);
    }
    spi_cs_high();

    // Manual bit-shifting for Big-Endian reconstruction
    data->ax   = (int16_t)((raw[0] << 8)  | raw[1]);
    data->ay   = (int16_t)((raw[2] << 8)  | raw[3]);
    data->az   = (int16_t)((raw[4] << 8)  | raw[5]);
    data->temp = (int16_t)((raw[6] << 8)  | raw[7]);
    data->gx   = (int16_t)((raw[8] << 8)  | raw[9]);
    data->gy   = (int16_t)((raw[10] << 8) | raw[11]);
    data->gz   = (int16_t)((raw[12] << 8) | raw[13]);
}
*/
//New inclusion 
int MPU9265_Init(void) {
    spi_init(200, 3); // 200kHz for init, Mode 3
    
    // 1. Reset and Wake
    mpu9265_write_reg(0x6B, 0x80); // H_RESET
    for(volatile int i=0; i<100000; i++);
    mpu9265_write_reg(0x6B, 0x01); // Clock Source Gyro Z
    
    // 2. Disable I2C Interface (Lock to SPI)
    // Set I2C_IF_DIS and I2C_MST_EN in USER_CTRL
    mpu9265_write_reg(REG_USER_CTRL, 0x30); 
    
    // 3. Setup Internal I2C Master to read Magnetometer
    mpu9265_write_reg(REG_I2C_MST_CTRL, 0x0D); // 400kHz I2C speed
    
    // 4. Configure Magnetometer (AK8963) via MPU's I2C Master
    // Write 0x16 (16-bit res, 100Hz continuous) to AK8963 CNTL1
    mpu9265_write_reg(REG_I2C_SLV0_ADDR, AK8963_I2C_ADDR);
    mpu9265_write_reg(REG_I2C_SLV0_REG, AK8963_CNTL1);
    mpu9265_write_reg(REG_I2C_SLV0_CTRL, 0x81); // Write 1 byte
    mpu9265_write_reg(0x63, 0x16); // Data to write (I2C_SLV0_DO)

    // 5. Set up Continuous Read of Magnetometer
    // This tells the MPU to automatically poll the Mag and put data in EXT_SENS registers
    mpu9265_write_reg(REG_I2C_SLV0_ADDR, AK8963_I2C_ADDR | 0x80); // Read mode
    mpu9265_write_reg(REG_I2C_SLV0_REG, AK8963_HXL);
    mpu9265_write_reg(REG_I2C_SLV0_CTRL, 0x87); // Read 7 bytes (6 data + 1 ST2 status)
    
    return 0;
}

void MPU9265_ReadAll(MPU9265_Data *data) {
    uint8_t raw[21]; // 14 (Accel/Gyro/Temp) + 7 (Mag)
    
    spi_cs_low();
    spi_transfer(0x3B | MPU9265_READ_BIT); // Start at Accel X
    for (int i = 0; i < 14; i++) raw[i] = spi_transfer(0x00);
    spi_cs_high();

    // The Mag data is now waiting in the External Sensor registers
    spi_cs_low();
    spi_transfer(REG_EXT_SENS_DATA_00 | MPU9265_READ_BIT);
    for (int i = 14; i < 21; i++) raw[i] = spi_transfer(0x00);
    spi_cs_high();

    // Reconstruct Accel/Gyro
    data->ax = (int16_t)(raw[0] << 8 | raw[1]);
    data->ay = (int16_t)(raw[2] << 8 | raw[3]);
    data->az = (int16_t)(raw[4] << 8 | raw[5]);
    
    // Convert Temperature to Celsius
    int16_t t_raw = (int16_t)(raw[6] << 8 | raw[7]);
    //data->temp_c = ((float)t_raw - 21.0f) / 333.87f + 21.0f;
    // Result is in centidegrees (e.g., 2525 = 25.25°C)
    data->temp_c = (((int32_t)t_raw - 0) * 100) / 334 + 2100;
    //gyro
    data->gx = (int16_t)(raw[8] << 8 | raw[9]);
    data->gy = (int16_t)(raw[10] << 8 | raw[11]);
    data->gz = (int16_t)(raw[12] << 8 | raw[13]);

    // Reconstruct Magnetometer (Note: AK8963 is Little Endian!)
    data->mx = (int16_t)(raw[15] << 8 | raw[14]);
    data->my = (int16_t)(raw[17] << 8 | raw[16]);
    data->mz = (int16_t)(raw[19] << 8 | raw[18]);
}