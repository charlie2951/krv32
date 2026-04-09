#include "uart.h"
#include "delay.h"
#include "mpu9265.h"

//helper function to print temp via uart in 'C
void print_temp(int32_t temp_val) {
    if (temp_val < 0)
    {
        uart_send(UART1, '-');
        temp_val = -temp_val;
    }
    
    int32_t whole = temp_val / 100;  // 295 / 10 = 29
    int32_t decimal = temp_val % 100; // 295 % 10 = 5
    uart_sendnumber(UART1, whole);
    uart_sendline(UART1,".");
    uart_sendnumber(UART1, decimal);
   
}

int main(void) {
    MPU9265_Data sensor;
    int status;

    // Initialize SPI and Sensor
    status = MPU9265_Init();
    
    if (status != 0) {
        uart_sendline(UART1,"Error: MPU9265 not found!\r\n");
        while(1); 
    }
    uart_sendline(UART1,"MPU9265 Initialized Successfully.\r\n");
    uart_sendline(UART1,"AccX, AccY, AccZ | GyroX, GyroY, GyroZ | MagX, MagY, MagZ | Temp('C)\r\n");
    while (1) {
        MPU9265_ReadAll(&sensor);

        /* Note: At +/- 2g sensitivity:
           - 16384 represents 1G
           - 0 represents 0G
           
           At +/- 250 dps sensitivity:
           - 131 represents 1 degree per second
        */
        
        // Placeholder for your processing logic
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
        uart_sendline(UART1,"\r\n");
        // Dummy delay
        delay(500000);
    }

    return 0;
}