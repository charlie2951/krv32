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
#include "uart.h"
#include "delay.h"
#define I2C I2C0 //select which I2C is used
int main(void)
{
    mpu_init(&I2C);

    uint8_t id = mpu_whoami(&I2C);
 uart_sendline(UART1, "\r\nDevice ID: ");
    uart_sendnumber(UART1,id);
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
   // mpu_read_accel(&I2C, &ax, &ay, &az);

    while (1){
        mpu_read_accel(&I2C, &ax, &ay, &az);
        mpu_read_gyro(&I2C, &gx, &gy, &gz);
        uint16_t temperature=(mpu_read_temp(&I2C)/340)+36;//converting raw temp data to 'C
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
        uart_sendline(UART1,"'C");
        delay(2000000);
    }

    return 0;
    }





