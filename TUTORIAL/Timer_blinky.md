### Blink one LED using hardware timer 

This tutorial shows how to use the hardware timer to generate a delay and to blink one LED. You must include the appropriate header 
file to access the timer and other API
```
#include <stdint.h>
#include "timer.h"
#include "gpio.h"
#include "uart.h"
#define TIMER TIMER1 //using timer-1
int main(void)
{ 
  
  pinMode(9, OUTPUT); // configure gpio-9 as output
  
  
    while(1)
    {
         digitalWrite(9,1); //write 1 to gpio pin
         uart_sendline(UART1,"writing high \r\n"); //print on UART
         timer_delay_ms(TIMER,1000); //timer based delay of 1sec
        digitalWrite(9,0); //writing 0
        uart_sendline(UART1,"writing low \r\n");
        timer_delay_ms(TIMER,2000); //timer based delay of 2sec
    }

    return 0;
}
```
Compile the code and upload the firmware. Then open any serial terminal application such as Putty/TeraTerm and configure the port as 115200.
See the printed status on UARt and LED status on board. Also, a simple python script is given `read_serial.py` to display the data coming to serial port.
You can run `python read_serial.py <COM_PORT_NO>` in your terminal to display the status . 
