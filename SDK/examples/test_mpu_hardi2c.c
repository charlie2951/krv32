#include <stdint.h>
#include "mpu.h"
#include "uart.h"
#include "delay.h"
int main(void)
{
    uint8_t id;
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    // Initialize I2C (100kHz @ 100MHz clock)
   // i2c_init(499);//for 100KHz i2c
   // i2c_init(124);//for 400KHz i2c

    // Initialize MPU6050
    mpu_init();

    // Check device ID
    id = mpu_whoami();   // Should read 0x68
    uart_sendline(UART1, "\r\nDevice ID: ");
    uart_sendnumber(UART1,id);
    while (1)
    {
        mpu_read_accel(&ax, &ay, &az);
        mpu_read_gyro(&gx, &gy, &gz);
        uint16_t temperature=(mpu_read_temp()/340)+36;//converting raw temp data to 'C
        // Place breakpoint here to inspect values
        // Print accel //
        uart_sendline(UART1, "\r\nACC X:");
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
       uart_sendnumber(UART1,gy);
        uart_sendline(UART1, " Z:");
        uart_sendnumber(UART1,gz);
        //print temp
        uart_sendline(UART1, "  Temp: ");
        uart_sendnumber(UART1,temperature);
        uart_sendline(UART1," 'C");
        delay(2000000);
    }

    return 0;
}