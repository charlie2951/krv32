#include "mpu9265.h"
#include "spi.h"
#include "i2c.h"

// State tracking
static mpu_interface_t _interface;
static i2c_t* _i2c_bus = 0;

#define MPU9265_ADDR      0x68
#define AK8963_ADDR       0x0C
#define READ_BIT          0x80

// =====================================================
// Internal Abstraction Layer
// =====================================================
static void write_reg(uint8_t device_addr, uint8_t reg, uint8_t val) {
    if (_interface == MPU_INTERFACE_SPI) {
        spi_cs_low();
        spi_transfer(reg);
        spi_transfer(val);
        spi_cs_high();
    } else {
        i2c_write(_i2c_bus, device_addr, reg, val);
    }
}

static uint8_t read_reg(uint8_t device_addr, uint8_t reg) {
    if (_interface == MPU_INTERFACE_SPI) {
        uint8_t val;
        spi_cs_low();
        spi_transfer(reg | READ_BIT);
        val = spi_transfer(0x00);
        spi_cs_high();
        return val;
    } else {
        return i2c_read(_i2c_bus, device_addr, reg);
    }
}

// =====================================================
// Main Driver
// =====================================================
int MPU9265_Init(mpu_interface_t interface, void* i2c_inst) {
    _interface = interface;
    _i2c_bus = (i2c_t*)i2c_inst;

    if (_interface == MPU_INTERFACE_SPI) {
        spi_init(200, 3);
        spi_cs_high();
    } else {
        // I2C already initialized in main.c typically
    }

    // 1. Reset and Wake
    write_reg(MPU9265_ADDR, 0x6B, 0x80); 
    for(volatile int i=0; i<100000; i++);
    write_reg(MPU9265_ADDR, 0x6B, 0x01); 

    if (_interface == MPU_INTERFACE_SPI) {
        // SPI Mode: Setup Internal I2C Master to poll Magnetometer
        write_reg(MPU9265_ADDR, 0x6A, 0x30); // Disable I2C IF, Enable I2C Master
        write_reg(MPU9265_ADDR, 0x24, 0x0D); // 400kHz
        
        // Power up AK8963 via MPU Master
        write_reg(MPU9265_ADDR, 0x25, AK8963_ADDR);
        write_reg(MPU9265_ADDR, 0x26, 0x0A); // CNTL1
        write_reg(MPU9265_ADDR, 0x63, 0x16); // 16-bit, 100Hz
        write_reg(MPU9265_ADDR, 0x27, 0x81); // Write 1 byte

        // Set Continuous Read
        write_reg(MPU9265_ADDR, 0x25, AK8963_ADDR | READ_BIT);
        write_reg(MPU9265_ADDR, 0x26, 0x03); // HXL
        write_reg(MPU9265_ADDR, 0x27, 0x87); // Read 7 bytes
    } else {
        // I2C Mode: Enable Bypass to talk to Mag directly
        write_reg(MPU9265_ADDR, 0x37, 0x02); 
        write_reg(AK8963_ADDR, 0x0A, 0x16); // Power up Mag
    }

    return 0;
}

void MPU9265_ReadAll(MPU9265_Data *data) {
    uint8_t raw[21];

    if (_interface == MPU_INTERFACE_SPI) {
        // Efficient SPI Burst Read
        spi_cs_low();
        spi_transfer(0x3B | READ_BIT);
        for (int i = 0; i < 14; i++) raw[i] = spi_transfer(0x00);
        spi_cs_high();

        spi_cs_low();
        spi_transfer(0x49 | READ_BIT); // EXT_SENS_DATA_00
        for (int i = 14; i < 21; i++) raw[i] = spi_transfer(0x00);
        spi_cs_high();
    } else {
        // I2C Individual Reads (as per your current API limitations)
        for (int i = 0; i < 14; i++) raw[i] = read_reg(MPU9265_ADDR, 0x3B + i);
        for (int i = 0; i < 7; i++)  raw[14 + i] = read_reg(AK8963_ADDR, 0x03 + i);
    }

    // Reconstruction (Same for both)
    data->ax = (int16_t)(raw[0] << 8 | raw[1]);
    data->ay = (int16_t)(raw[2] << 8 | raw[3]);
    data->az = (int16_t)(raw[4] << 8 | raw[5]);
    
    int16_t t_raw = (int16_t)(raw[6] << 8 | raw[7]);
    data->temp_c = (((int32_t)t_raw) * 100) / 334 + 2100;

    data->gx = (int16_t)(raw[8] << 8 | raw[9]);
    data->gy = (int16_t)(raw[10] << 8 | raw[11]);
    data->gz = (int16_t)(raw[12] << 8 | raw[13]);

    data->mx = (int16_t)(raw[15] << 8 | raw[14]); // Little Endian
    data->my = (int16_t)(raw[17] << 8 | raw[16]);
    data->mz = (int16_t)(raw[19] << 8 | raw[18]);
}