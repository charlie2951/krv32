/*
Main header for KRV-32 SOC
Version-1.0.1
MIT License
Copyright (c) 2024-2025 Dr. Subir Kr. Maity, School of Electronics Engg, KIIT University
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

#include <stdint.h>
#include <stdlib.h>
//*** LED Address mapping */
#define LED_ADDR 0x10000000   // starting addr of LED
#define LED_DATA *((volatile unsigned int *)(LED_ADDR ))
//********** UART Transmitter Register address map *******
#define UART_TXDATA_ADDR 0x20000000   // starting addr of UART DATA REG
#define UART_TXDATA *((volatile unsigned int *)(UART_TXDATA_ADDR ))
#define UART_TXCTRL_ADDR 0x20000004   // starting addr of UART CTRL REG
#define UART_TXCTRL *((volatile unsigned int *)(UART_TXCTRL_ADDR ))
#define UART_TXSTATUS_ADDR 0x20000008   // starting addr of UART status REG
#define UART_TXSTATUS *((volatile unsigned int *)(UART_TXSTATUS_ADDR ))
//******UART Receiver Register address mapping**************
#define UART_RXDATA_ADDR 0x20000010   // starting addr of UART DATA REG
#define UART_RXDATA *((volatile unsigned int *)(UART_RXDATA_ADDR ))
#define UART_RXCTRL_ADDR 0x20000014   // starting addr of UART CTRL REG
#define UART_RXCTRL *((volatile unsigned int *)(UART_RXCTRL_ADDR ))
#define UART_RXSTATUS_ADDR 0x20000018   // starting addr of UART status REG
#define UART_RXSTATUS *((volatile unsigned int *)(UART_RXSTATUS_ADDR ))
#define BOOTMEM_ADDR 0x00080000   // starting addr of Bootloader

//Delay function
void delay(uint32_t cycles) {
  volatile uint32_t count = 0; // volatile to prevent compiler optimization

  while (count < cycles) {
    count++;
  }
}


//uart function to send a single character
void uart_send(uint8_t my_char)
{
    while(UART_TXSTATUS==0) ;
           
    UART_TXDATA = my_char;
    UART_TXCTRL = 1;
    UART_TXCTRL = 0;
   
}
//UART function to send a string 
void uart_sendline(uint8_t *my_str)
{
    for (uint8_t i = 0; my_str[i] != '\0'; i++)
    {
        uart_send(my_str[i]);
       
    }
}

//UART Receive 
volatile uint32_t uart_receive()
{
	UART_RXCTRL = 1;//enable receiver
   UART_RXCTRL = 0;//disable receiver
   	while(UART_RXSTATUS==0) ;//wait if busy receiving data
    //UART_RXCTRL = 0;//disable receiver
           
   	return UART_RXDATA ;
    
}

