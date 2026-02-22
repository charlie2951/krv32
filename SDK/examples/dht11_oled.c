/**
 DHT11 interfacing and display data in OLED SSD1306
 */
#include <stdint.h>
#include "delay.h"
#include "dht11.h"
#include "i2c_master.h"
#include "ssd.h"
#include "my_string.h"
#define I2C I2C0
int main(void){
    uint32_t temperature, humidity;
    char tmp[4],hum[4];
    dht11_init(4);//DHT pin is GPIO-4 i.e. pin7 of JB header
    //softi2c_init();
    ssd1306_init(&I2C);
    ssd1306_set_cursor(0, 2);
    ssd1306_write_string("Temp & Humidity Stat");
    ssd1306_set_cursor(3, 2);
    ssd1306_write_string("Temp:");
    ssd1306_set_cursor(6, 2);
    ssd1306_write_string("Humidity:");
    while(1) {
    
    int ret = dht11_read_temperature_humidity(&temperature, &humidity);
    my_int2str(temperature,tmp);//convert data to string
    my_int2str(humidity,hum);

    if (ret == 0)
    { //display on successful read data from sensor

        //send data to oled
        ssd1306_set_cursor(3, 60);
        ssd1306_write_string(tmp);
        ssd1306_set_cursor(6, 60);
        ssd1306_write_string(hum);
        //display debug info on UART
        /*
        uart_sendline(UART1,"Temp: ");
        uart_sendnumber(UART1,temperature);
        uart_sendline(UART1," Humidity: ");
        uart_sendnumber(UART1,humidity);
        uart_sendline(UART1,"\r\n");
       */
    }
    /*
    else
    {
        uart_sendline(UART1,"Error\r\n");
    }*/

    delay(10000000);
}
}


