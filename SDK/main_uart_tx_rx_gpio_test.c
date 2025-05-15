// C code to test UART receiver-transmitter along with gpio

#include <krv32.h>


int main()
{
   
	uart_sendline("Press any key between 0 to 9: \n\r");
      
    while (1)
    {
	volatile uint32_t rx_data = uart_receive();
        GCSR->GPIO_0 =  rx_data - 48; //convert to decimal digit of num
         
        uart_sendline("You pressed:  ");
        uart_send((char)rx_data);
	uart_sendline("\n\r ");
        delay(100000);
        
    }
}