#include "mpu9265.h"
#include "i2c.h"
#include "uart.h"
#include "spi.h"
#include "delay.h"
#include "printf.h" //for higher level API, print support via uart1
/*
//helper function to print temp via uart in 'C
void print_temp(int32_t temp_val) {
    if (temp_val < 0)
    {
        uart_send(UART1, '-');
        temp_val = -temp_val;
    }
    
    int32_t whole = temp_val / 100;  // 295 / 10 = 29
    int32_t decimal = temp_val % 100; // 295 % 10 = 5
    //uart_sendnumber(UART1, whole);
   // uart_sendline(UART1,".");
   // uart_sendnumber(UART1, decimal);
  //new API reduces the burden of printing
  printf("%d.%d", whole,decimal);
   
}
*/

int main() {
    MPU9265_Data sensor;

    // To use SPI:
   // int status = MPU9265_Init(MPU_INTERFACE_SPI, 0);
    // OR to use I2C:
     i2c_init(&I2C0, 499);
    int status= MPU9265_Init(MPU_INTERFACE_I2C, &I2C0);
if (status != 0) {
       // uart_sendline(UART1,"Error: MPU9265 not found!\r\n");
  printf("Error: MPU9265 not found!\r\n");
        while(1); 
    }
   printf("MPU9265 Initialized Successfully.\r\n");
   printf("AccX, AccY, AccZ | GyroX, GyroY, GyroZ | MagX, MagY, MagZ | Temp('C)\r\n");
   
    

    while(1) {
        MPU9265_ReadAll(&sensor);
        // Print data...
        //Accelerometer data
        int x_accel = sensor.ax;
        int y_accel = sensor.ay;
        int z_accel = sensor.az;
        //Gyro data
        int g_x = sensor.gx;
        int g_y = sensor.gy;
        int g_z = sensor.gz;
        //Magnetometer data
        int m_x = sensor.mx;
        int m_y = sensor.my;
        int m_z = sensor.mz;
        //print
      /*
        uart_sendnumber(UART1,x_accel);
        uart_sendline(UART1," |");
        uart_sendnumber(UART1,y_accel);
        uart_sendline(UART1," |");
        uart_sendnumber(UART1,z_accel);
        uart_sendline(UART1," |");
        uart_sendnumber(UART1,g_x);
        uart_sendline(UART1," |");
        uart_sendnumber(UART1,g_y);
        uart_sendline(UART1," |");
        uart_sendnumber(UART1,g_z);
        uart_sendline(UART1," |");
        uart_sendnumber(UART1,m_x);
        uart_sendline(UART1," |");
        uart_sendnumber(UART1,m_y);
        uart_sendline(UART1," |");
        uart_sendnumber(UART1,m_z);
        uart_sendline(UART1," |");
        print_temp(sensor.temp_c);
        uart_sendline(UART1,"\r\n"); */
      printf("\r\na_x:%d, a_y:%d, a_z:%d, g_x:%d, g_y=%d, g_z=%d, m_x=%d, m_y=%d, m_z=%d, Temp=%d.%d", x_accel,y_accel,z_accel,g_x,g_y,g_z,m_x,m_y,m_z,sensor.temp_c/100,sensor.temp_c%100);
        // Dummy delay
        delay(2000000);
    }
    return 0;
}