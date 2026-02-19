#include <stdint.h>
#include "timer.h"
#include "gpio.h"
#include "delay.h"
#include "uart.h"

//main func
int main(void)
{
        static uint32_t count_val;
    //Initialize timer in free running counter mode to count pulse
    //Note: after initialization, immediately the counting will start
    timer_init(1);//counter mode
      
    while(1)
    {
     delay(2000000);
    timer_getcount(&count_val);
     uart_sendline(UART1,"Count value:");
     uart_sendnumber(UART1,count_val);
     uart_sendline(UART1,"\r\n");
    }

    return 0;
}
