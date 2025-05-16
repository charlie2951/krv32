# KRV-32 SDK User Manual
The ** krv32. h ** header contains all necessary functions for accessing GPIOs, UART, and PWM pins. 

### Useful C functions
***delay(unsigned int count)***  Provides a commin delay function, not optimized for clock frequency variation. At 27MHz Clock, each count provides approx. 1uS delay. <p>
***void digitalwrite(unsigned int value)*** write 8 bit data to LED port. Note LED port is mapped to address 0x1000_0000.<p>
***void uart_send(uint8_t my_char)*** used to print/transmit a character via serial terminal TX line. Current version of UART transmitter and receiver support only 9600 baud. <p>
***void uart_sendline(uint8_t *my_str)*** used to print/transmit a string via TX line of UART. <p>
***void pwm_begin(volatile uint32_t channel_no, volatile uint32_t duty_cycle)*** used to generate PWM waveform at a particular channel specified by channel_no and duty cycle specified by duty_cycle. Note: channel number is 0-3 and duty cycle value is in range of 0-255 i.e. 0 indicates 0% and 255 indicates 100% duty cycle. The frequency of PWM signal in each channel is fixed to 500Hz. Note that master clock freq =100MHz is used. If your FPGA board has different clock frequency then change the Master clock frequency  **parameter CLK_FREQ = 100_000_000;** in **pwm.v** file.<p>

## Example-1
Generate PWM signal at 4 different channel with varying duty cycle <p>
```c
// C code to test PWM on 4 channel
// Make sure that krv32.h is present inside "include" directory
#include <krv32.h>

int main()
{
    delay(1000000);
   	uart_sendline("PWM Module test \n\r");
      //enable PWM on ch0 to 3
      pwm_begin(0,100);//pwm_begin(ch no(0 to 3), value:1 or 0)
      pwm_begin(1,50);
      pwm_begin(2,70);
      pwm_begin(3,30);
      
    delay(1000000);
	uart_sendline("Check LED for PWM output\n\r");
        
    
}
```

  Here the pwm channel is activated using **pwm_begin(ch no, duty_cycle)**. The 1st channel having duty cycle count of 100 i.e. (100/255)*100% = 40% and so on.
