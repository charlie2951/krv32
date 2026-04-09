#include "i2c.h"
#include "uart.h"
#include "adxl345.h"
#include <stdint.h>
#include "delay.h"
int main() {
    adxl345_data_t accel;
    
    // 1. Init I2C Hardware (assuming system clock 50MHz for 100kHz I2C)
    // divider = (50,000,000 / (2 * 100,000)) - 1 = 249
    i2c_init(&I2C0, 249);
    
    // 2. Init Sensor
    if (adxl345_i2c_init(&I2C0) != 0) {
        uart_sendline(UART1,"Handle Error: Sensor not responding!\r\n");
        while(1);//stop execution, hang in loop
    }

    while(1) {
        // 3. Read Data
        adxl345_i2c_read(&I2C0, &accel);
        uart_sendline(UART1,"ADXL345 Data: X=");
        uart_sendnumber(UART1,accel.x);
        uart_sendline(UART1, " y=");
        uart_sendnumber(UART1,accel.y);
        uart_sendline(UART1, " y=");
        uart_sendnumber(UART1,accel.z);
        uart_sendline(UART1, "\r\n");

        // Use data...
        delay(500000);//0.5sec delay
    }
}