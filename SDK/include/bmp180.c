#include "bmp180.h"
#include "uart.h"
static bmp180_calib_t cal;
static int32_t b5; // Global helper for pressure calculation

// Helper to read 16-bit values (Big Endian)
/*
static int16_t read_16(i2c_t *i2c, uint8_t reg) {
    uint8_t msb = i2c_read(i2c, BMP180_ADDR, reg);
    uint8_t lsb = i2c_read(i2c, BMP180_ADDR, reg + 1);
    return (int16_t)((msb << 8) | lsb);
}
    */
static int16_t read_16(i2c_t *i2c, uint8_t reg) {
    uint8_t msb = i2c_read(i2c, BMP180_ADDR, reg);
    uint8_t lsb = i2c_read(i2c, BMP180_ADDR, reg + 1);
    // Combine as unsigned first, then cast to signed
    return (int16_t)(((uint16_t)msb << 8) | (uint16_t)lsb);
}

int bmp180_init(i2c_t *i2c) {
    if (i2c_read(i2c, BMP180_ADDR, BMP180_CHIP_ID_REG) != 0x55) return -1;

    // Read calibration data (0xAA to 0xBF)
    cal.ac1 = read_16(i2c, 0xAA);
    cal.ac2 = read_16(i2c, 0xAC);
    cal.ac3 = read_16(i2c, 0xAE);
    cal.ac4 = (uint16_t)read_16(i2c, 0xB0);
    cal.ac5 = (uint16_t)read_16(i2c, 0xB2);
    cal.ac6 = (uint16_t)read_16(i2c, 0xB4);
    cal.b1  = read_16(i2c, 0xB6);
    cal.b2  = read_16(i2c, 0xB8);
    cal.mb  = read_16(i2c, 0xBA);
    cal.mc  = read_16(i2c, 0xBC);
    cal.md  = read_16(i2c, 0xBE);
    
    return 0;
}

int32_t bmp180_get_raw_temp(i2c_t *i2c) {
    i2c_write(i2c, BMP180_ADDR, BMP180_CTRL_MEAS_REG, BMP180_GET_TEMP);
    // Wait at least 4.5ms (you should implement a real delay here)
    for(volatile int i=0; i<50000; i++); 
    //return (int32_t)read_16(i2c, BMP180_ADC_OUT_MSB);
    uint16_t raw_u16 = (uint16_t)read_16(i2c, BMP180_ADC_OUT_MSB);
    return (int32_t)raw_u16;
}

int32_t bmp180_calculate_temp(int32_t ut) {
    int32_t x1 = (ut - cal.ac6) * cal.ac5 >> 15;
    int32_t x2 = (cal.mc << 11) / (x1 + cal.md);
    b5 = x1 + x2;
    return (b5 + 8) >> 4; // Result in 0.1 deg C
         
}

int32_t bmp180_get_raw_pressure(i2c_t *i2c, bmp180_oss_t oss) {
    i2c_write(i2c, BMP180_ADDR, BMP180_CTRL_MEAS_REG, (BMP180_GET_PRESSURE + (oss << 6)));
    // Wait depends on OSS (4.5ms to 25.5ms)
    for(volatile int i=0; i<200000; i++); 
    // Example for OSS = 0 (Ultra Low Power)
    uint8_t msb  = i2c_read(i2c, BMP180_ADDR, 0xF6); // Most Significant Byte
    uint8_t lsb  = i2c_read(i2c, BMP180_ADDR, 0xF7); // Least Significant Byte
    uint8_t xlsb = i2c_read(i2c, BMP180_ADDR, 0xF8); // Extended Least Significant Byte

    // The correct way to combine them for 19-bit resolution:
    int32_t up = (int32_t)((((uint32_t)msb << 16) | ((uint32_t)lsb << 8) | (uint32_t)xlsb) >> (8 - oss));
    return up;
}

/*
int32_t bmp180_calculate_pressure(int32_t up, bmp180_oss_t oss) {
    int32_t b6, x1, x2, x3, b3, p;
    uint32_t b4, b7;

    b6 = b5 - 4000;
    x1 = (cal.b2 * (b6 * b6 >> 12)) >> 11;
    x2 = cal.ac2 * b6 >> 11;
    x3 = x1 + x2;
    //b3 = (((int32_t)cal.ac1 * 4 + x3) << oss + 2) >> 2;
    // Break it down to ensure the compiler uses signed 32-bit math at every step
    int32_t term1 = ((int32_t)cal.ac1 * 4); 
    int32_t term2 = term1 + x3;
    b3 = (( (term2 << oss) + 2) >> 2);
    x1 = cal.ac3 * b6 >> 13;
    x2 = (cal.b1 * (b6 * b6 >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (uint32_t)cal.ac4 * (uint32_t)(x3 + 32768) >> 15;
    b7 = ((uint32_t)up - b3) * (50000 >> oss);
    if (b7 < 0x80000000) p = (b7 * 2) / b4;
    else p = (b7 / b4) * 2;
    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    p = p + ((x1 + x2 + 3791) >> 4);
    //debug
    uart_sendline(UART1," B3: ");
    uart_sendnumber(UART1,b3);
    uart_sendline(UART1," B4: ");
    uart_sendnumber(UART1,b4);
    uart_sendline(UART1," cal.ac1: ");
    uart_sendnumber(UART1,cal.ac1);
    return p; // Pressure in Pa
}
*/
int32_t bmp180_calculate_pressure(int32_t up, bmp180_oss_t oss) {
    int32_t x1, x2, x3, b3, b6, p;
    uint32_t b4, b7; // Use unsigned 32-bit for the big math

    b6 = b5 - 4000;
    
    // X1, X2, X3 calculations
    x1 = (cal.b2 * (b6 * b6 >> 12)) >> 11;
    x2 = (cal.ac2 * b6) >> 11;
    x3 = x1 + x2;
    
    // B3 calculation (OSS=1)
    b3 = ((((int32_t)cal.ac1 * 4 + x3) << oss) + 2) >> 2;

    // B4 calculation
    x1 = (cal.ac3 * b6) >> 13;
    x2 = (cal.b1 * (b6 * b6 >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (uint32_t)cal.ac4 * (uint32_t)(x3 + 32768) >> 15;

    // B7 calculation using UNSIGNED 32-bit
    // (82354 - 13860) * 25000 = 1,712,350,000 (Fits in uint32_t!)
    b7 = (uint32_t)((uint32_t)up - (uint32_t)b3) * (50000 >> oss);

    // Initial pressure calculation
    // Since b7 * 2 might exceed 4 billion, we divide first or use the Bosch logic:
    if (b7 < 0x80000000) {
        p = (int32_t)((b7 << 1) / b4);
    } else {
        p = (int32_t)((b7 / b4) << 1);
    }

    // Final fine-tuning
    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    p = p + ((x1 + x2 + 3791) >> 4);

    return p;
}
/**
 * @brief Calculates altitude in meters.
 * @param pressure Current pressure in Pa
 * @param baseline_pressure Pressure at "zero" meters (e.g., 101325)
 * @return Altitude in meters
 */
int32_t bmp180_get_altitude(int32_t pressure, int32_t baseline_pressure) {
    if (pressure > baseline_pressure) return 0;
    
    // Integer-safe linear approximation for low altitudes
    // 1 meter change is approx 12 Pascals
    return (baseline_pressure - pressure) * 10 / 120;
}