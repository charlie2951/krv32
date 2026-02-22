#include <stdint.h>
#include "timer.h"
#include "gpio.h"
#include "uart.h"
#define TIMER TIMER0 //using timer-0
int main(void)
{ 
  
  pinMode(9, OUTPUT); //gpio-9 as output
  
  
    while(1)
    {
         digitalWrite(9,1);
         uart_sendline(UART1,"writing high \r\n");
         timer_delay_ms(TIMER,1000);
        digitalWrite(9,0);
        uart_sendline(UART1,"writing low \r\n");
        timer_delay_ms(TIMER,2000);
    }

    return 0;
}
