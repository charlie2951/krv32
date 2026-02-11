/*
	MIT License

Copyright (c) 2024-2026 Subir Maity

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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
