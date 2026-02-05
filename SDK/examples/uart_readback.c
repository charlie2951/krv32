//reading data from PC keystroke via UART and print the same in Serial terminal
#include "uart.h"
#include "delay.h"

// Driver program to test above function
int main(void)
{
  uart_init(UART1);//initialize uart1
  
  uart_sendline(UART1,"RISC-V UART is listenting and ready to transmit or receive\n\r");
    while(1){
    uart_sendline(UART1,"Press any key in keyboard: \n\r");
    //Receive data from UART and display to LED
    //subtract 48 to convert ASCII to number
     uint8_t rx_data =uart_receive(UART1);
    uart_sendline(UART1,"You pressed:  ");
    uart_send(UART1,(char)rx_data);
	uart_sendline(UART1,"\n\r ");
    delay(100000);

  }
	

	return 0;
}