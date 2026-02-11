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
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "delay.h"
#include "softi2c.h"
#include "mpu6050.h"

// GPIO->0 used for SDA and GPIO-1 for SCL, already defined in Soft_i2c header
// Simple integer print buffer //
static char buf[64];

//ascii conversion
volatile uint8_t byte_to_hex_ascii(uint8_t data)
{
    volatile uint8_t low_nibble  = data & 0x0F;
    volatile uint8_t ascii;
   
    // Convert low nibble
    if (low_nibble >= 0 && low_nibble <10)
        ascii = 48 + low_nibble;
    else
        ascii = 55 + low_nibble;

   return ascii;
}
//function to print hex as ascii in serial terminal
void print_data(int16_t data)
{
volatile uint32_t i;
volatile uint32_t  uart_txdata;
  
if(data < 0)
uart_send(UART1,'-');//print neg sign
uart_sendline(UART1,"0x");
for(i=0;i<4;i++) //send all 4 byte
  {
  //uart_txdata = (data >> (28 - (i * 4))) & 0x0F;  
  uart_txdata = (data >> (12 - (i * 4))) & 0x0F;

  volatile uint8_t ascii = byte_to_hex_ascii(uart_txdata);
  uart_send(UART1,ascii);//will print byte wise
  //delay(1000);
  }
}

//helper func replacement of snprintf()

static void print_int(int16_t val)
{
    static char tmp[8];
    int i = 0;

    if (val < 0)
    {
        uart_send(UART1, '-');
        val = -val;
    }

    do {
        tmp[i++] = '0' + (val % 10);
        val /= 10;
    } while (val);

    while (i--)
        uart_send(UART1, tmp[i]);
}



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
        //sanity test-1
        // int16_t val=mpu6050_who_am_i();//test passed
         // print_data(val);

        //sanity test2
        // int8_t axh = softi2c_read_reg(0x68, 0x3B);//passed
       //  int8_t axl = softi2c_read_reg(0x68, 0x3C);//passed
       //  int16_t val = (int16_t)((axh << 8) | axl);
       //  print_data(val);
        //  print_data(axh);
        // uart_sendline(UART1, ",: ");
        // print_data(axl);
       //  uart_sendline(UART1, "\r\n");
        // Read sensors //
        
         mpu6050_read_accel(&ax, &ay, &az);
         mpu6050_read_gyro(&gx, &gy, &gz);
         uint16_t temperature=(mpu6050_read_temp()/340)+36;//converting raw temp data to 'C

        // Print accel //
        uart_sendline(UART1, "ACC X:");
        print_int(ax);
        uart_sendline(UART1, " Y:");
        print_int(ay);
        uart_sendline(UART1, " Z:");
        print_int(az);
       // uart_sendline(UART1, "\r\n");
        

        // Print gyro //
        
        uart_sendline(UART1, " GYRO X:");
        print_int(gx);
        uart_sendline(UART1, " Y:");
        print_int(gy);
        uart_sendline(UART1, " Z:");
        print_int(gz);
       // uart_sendline(UART1, "\r\n");
        // Print Temperature raw data
        uart_sendline(UART1, "  Temp: ");
        print_int(temperature);
        uart_sendline(UART1," 'C");
        uart_sendline(UART1, "\r\n");
      //wait for some time to fetch next data byte
        delay(2000000);
    }
}


