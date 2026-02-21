### Displaying Temperature and Humidity data on SSD1306 OLED Display


**Steps:**
1. Connect SSD1306 to soft i2c port JB pin 1 (SDA) mapped to GPIO-0, and pin2(SCL) mapped to GPIO-1.
2. Connect DHT11 data line to JB pin-7 (GPIO4)
3. Build and upload the firmware <p>
**Sample demo code:**
```
/**
 DHT11 interfacing and display data on OLED display
 */
#include <stdint.h>
#include "delay.h"
#include "dht11.h"
#include "softi2c.h"
#include "ssd1306.h"
#include "my_string.h"

int main(void){
    uint32_t temperature, humidity;
    char tmp[4],hum[4];
    dht11_init(4);//DHT pin is GPIO-4 i.e. pin7 of JB header
    softi2c_init();
    ssd1306_init();
   // ssd1306_set_cursor(page no, pixel);//page no range 0-7, pixel 0-128
   //each char font having 5 pixel width
    ssd1306_set_cursor(0, 2);
    ssd1306_write_string("Temp & Humidity Stat");
    ssd1306_set_cursor(3, 2);
    ssd1306_write_string("Temp:");
    ssd1306_set_cursor(6, 2);
    ssd1306_write_string("Humidity:");
    while(1) {
    
    int ret = dht11_read_temperature_humidity(&temperature, &humidity);
   //convert temp and humidity data to string
    my_int2str(temperature,tmp);
    my_int2str(humidity,hum);

    if (ret == 0) //if DHT11 reading is valid
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



```
