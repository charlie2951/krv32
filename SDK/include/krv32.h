/*
Main header for KRV-32 SOC
Version-1.0.1
Copyright: Dr. S.K.Maity, School of Electronics Engg, KIIT University
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

