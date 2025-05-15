// C code to test PWM on 4 channel

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