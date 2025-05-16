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
#include <gpio_regs.h>
#include <uart_regs_rx.h>
#include <uart_regs_tx.h>
#include <pwm_regs.h>

//delay function
//count is in microsec (typical)
void delay(volatile uint32_t count)
{
    for (volatile uint32_t i = 0; i < count; i++)
        ;
}

//GPIO LED data display

void digitalwrite(uint32_t value)
{
GCSR->GPIO_0 = value;
}

//UART Receive 
volatile uint32_t uart_receive()
{
	UART_RX->U_CTRL_bf.START = 1;//enable receiver
   
   	while(UART_RX->U_STAT_bf.READY==0) ;//wait if busy receiving data
           
   	return UART_RX->U_DATA ;
    
}
//uart send a single character
void uart_send(uint8_t my_char)
{
    while(UCSR->U_STAT_bf.READY==0) ;
           
    UCSR->U_DATA = my_char;
    UCSR->U_CTRL_bf.START = 1;
   
}
//UARt send a string 
void uart_sendline(uint8_t *my_str)
{
    for (uint8_t i = 0; my_str[i] != '\0'; i++)
    {
        uart_send(my_str[i]);
       
    }
}

//PWM related functions
//PWM channel select: 0 to 3
//duty cycle is for selected channel : range 0 to 255
void pwm_begin(volatile uint32_t channel_no, volatile uint32_t duty_cycle)
{
if(channel_no==0){
PWM->CHANNEL_SEL0=1;
PWM->DUTY_CYCLE0=duty_cycle;
}

if(channel_no==1){
PWM->CHANNEL_SEL1=1;
PWM->DUTY_CYCLE1=duty_cycle;
}

if(channel_no==2){
PWM->CHANNEL_SEL2=1;
PWM->DUTY_CYCLE2=duty_cycle;
}

if(channel_no==3){
PWM->CHANNEL_SEL3=1;
PWM->DUTY_CYCLE3=duty_cycle;
}

}

