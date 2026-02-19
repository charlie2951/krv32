### This demo is about interfacing MPU6050 accelerometer and Gyro sensor 
Note: A software-based I2C implementation is used to communicate with MPU sensor. 
GPIO->0 used for SDA and GPIO-1 for SCL, already defined in Soft_i2c header. Connect the sensor pin SDA to PMOD JB pin-1 
and SCL to PMOD JB pin-2. Also, connect VDD and GND with the proper pins discussed earlier.

```
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "delay.h"
#include "softi2c.h"
#include "mpu6050.h"

// GPIO->0 used for SDA and GPIO-1 for SCL, already defined in Soft_i2c header
// Simple integer print buffer //
static char buf[64];


int main(void)
{
     static int16_t ax, ay, az;
     static int16_t gx, gy, gz;

    // Init peripherals //
    uart_init(UART1);          // Debug UART
    softi2c_init();

    uart_sendline(UART1, "\r\nMPU6050 Test Start\r\n");

    // Init MPU6050 //
    if (!mpu6050_init())
    {
        uart_sendline(UART1, "MPU6050 not detected!\r\n");
        while (1);
    }

    uart_sendline(UART1, "MPU6050 OK\r\n");

    while (1)
    {
               
         mpu6050_read_accel(&ax, &ay, &az);
         mpu6050_read_gyro(&gx, &gy, &gz);
         uint16_t temperature=(mpu6050_read_temp()/340)+36;//converting raw temp data to 'C

        // Print accel //
        uart_sendline(UART1, "ACC X:");
        uart_sendnumber(UART1,ax);
        uart_sendline(UART1, " Y:");
        uart_sendnumber(UART1,ay);
        uart_sendline(UART1, " Z:");
        uart_sendnumber(UART1,az);
       // uart_sendline(UART1, "\r\n");
        

        // Print gyro //
        
        uart_sendline(UART1, " GYRO X:");
        uart_sendnumber(UART1,gx);
        uart_sendline(UART1, " Y:");
        uart_sendnumber(UART1,gy)
        uart_sendline(UART1, " Z:");
        uart_sendnumber(UART1,gz);
       // uart_sendline(UART1, "\r\n");
        // Print Temperature raw data
        uart_sendline(UART1, "  Temp: ");
       uart_sendnumber(UART1,temperature);
        uart_sendline(UART1," 'C");
        uart_sendline(UART1, "\r\n");
      //wait for some time to fetch next data byte
        delay(2000000);
    }
}
```
compile the code and upload the firmware and see the acceleration and gyro data along with temperature in serial
monitor.
