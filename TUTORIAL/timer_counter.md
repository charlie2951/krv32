### Use of hardware timer as free running counter 
In this demo, the timer is configured as free running counter and after some interval, the count status is displayed ober serial monitor.
```
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
    timer_init(1);//configure timer as counter mode i.e. mode=1
      
    while(1)
    {
     delay(2000000); //software delay
    timer_getcount(&count_val);
     uart_sendline(UART1,"Count value:"); //print string
     uart_sendnumber(UART1,count_val); //print count value
     uart_sendline(UART1,"\r\n"); //print new line 
    }

    return 0;
}
```
After compilation and uploading the formware, see the serial monitor to check the count status.
