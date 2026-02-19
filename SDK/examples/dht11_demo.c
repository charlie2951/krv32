/**
 DHT11 interfacing demo
 */
#include <stdint.h>
#include "uart.h"
#include "delay.h"
#include "dht11.h"

int main(void){
    uint32_t temperature, humidity;
    uart_init(UART1);
    dht11_init(4);//DHT pin is GPIO-4 i.e. pin7 of JB header
    uart_sendline(UART1,"setup done\r\n");
    delay(5000000);

    while(1) {

    
    int ret = dht11_read_temperature_humidity(&temperature, &humidity);
    

    if (ret == 0)
    {
        uart_sendline(UART1,"Temp: ");
        uart_sendnumber(UART1,temperature);
        uart_sendline(UART1," Humidity: ");
        uart_sendnumber(UART1,humidity);
        uart_sendline(UART1,"\r\n");
    }
    else
    {
        uart_sendline(UART1,"Error\r\n");
    }

    delay(5000000);
}
}


