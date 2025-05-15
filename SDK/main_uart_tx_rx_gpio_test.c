/* C code to test UART receiver-transmitter along with gpio
This design is to accespt input from serial terminal such as PUTTY and
print the pressed key and display the same in the on-board LED if any number
between 0-9 is pressed.
All pressed characters are displayed on serial terminal
Check the serial port number properly and connect to 9600 baud
*/

#include <krv32.h>


int main()
{
   	//support 9600 baud rate only
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
