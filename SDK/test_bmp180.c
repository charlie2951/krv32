#include "bmp180.h"
#include "delay.h"
#include "uart.h"

//helper function to print temp via uart in 'C
void print_temp(int32_t temp_val) {
    int32_t whole = temp_val / 10;  // 295 / 10 = 29
    int32_t decimal = temp_val % 10; // 295 % 10 = 5
    uart_sendline(UART1,"Temp: ");
    uart_sendnumber(UART1, whole);
    uart_sendline(UART1,".");
    uart_sendnumber(UART1, decimal);
    uart_sendline(UART1,"'C");
}
//helper func to print pressure in hPa via uart
void print_pressure_hpa(int32_t pa) {
    int32_t hpa_whole = pa / 100;     // 100866 / 100 = 1008
    int32_t hpa_decimal = pa % 100;   // 100866 % 100 = 66
    uart_sendline(UART1,"Pressure: ");
    uart_sendnumber(UART1, hpa_whole);
    uart_sendline(UART1,".");
    uart_sendnumber(UART1, hpa_decimal);
    uart_sendline(UART1,"hPa");
}
int main() {
    i2c_init(&I2C0, 249);
    if (bmp180_init(&I2C0) != 0){
        uart_sendline(UART1, "Unable to detect BMP180 sensor! \r\n ");
         while(1); // Error
    }
    uart_sendline(UART1, "BMP180 found in I2C bus! \r\n ");
       
    while(1) {
        
        int32_t raw_t = bmp180_get_raw_temp(&I2C0);
        int32_t temp = bmp180_calculate_temp(raw_t); // e.g., 255 = 25.5C

        int32_t raw_p = bmp180_get_raw_pressure(&I2C0, BMP180_STANDARD);
        int32_t pressure = bmp180_calculate_pressure(raw_p, BMP180_STANDARD);
       // 3. Calculate Altitude (Relative to Sea Level)
        int32_t altitude = bmp180_get_altitude(pressure, SEA_LEVEL_PRESSURE);
        // Use temp and pressure...
        uart_sendline(UART1, "\r\n");
        print_temp(temp);
        uart_sendline(UART1, "\r\n");
        print_pressure_hpa(pressure);
        uart_sendline(UART1, "\r\n");
        uart_sendline(UART1, "Altitude: ");
        uart_sendnumber(UART1,altitude);
        uart_sendline(UART1, "meter\r\n ");
         
        delay(2000000);//2sec delay
    } 
    return 0;
    
}