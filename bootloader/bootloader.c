// C program to test UART bootloader to load hex file into BRAM
// Incoming data from serial port via python script
//Load received data on BRAM
//Version-2.0

#include <stdint.h>
#include <stdlib.h>
#include "uart.h"
#include "gpio.h"
#include "seven_seg.h"
#include "delay.h"

//BRAM mem location
#define BRAM_BASE   0x00000000
volatile uint32_t *bram = (uint32_t *)BRAM_BASE;

//ascii conversion
volatile uint8_t byte_to_hex_ascii(uint8_t data)
{
    volatile uint8_t low_nibble  = data & 0x0F;
    volatile uint8_t ascii;
   
    // Convert low nibble
    if (low_nibble >= 0 && low_nibble <10)
        ascii = 48 + low_nibble;
    else
        ascii = 55 + low_nibble;

   return ascii;
}




uint32_t hex_to_u32(char *buf)
{
    uint32_t val = 0;
    for (int i = 0; i < 8; i++) {
        char c = buf[i];
        val <<= 4;
        if (c >= '0' && c <= '9')
            val |= (c - '0');
        else if (c >= 'a' && c <= 'f')
            val |= (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            val |= (c - 'A' + 10);
    }
    return val;
}

//function to pront hex as ascii in serial terminal
void print_hex(uint32_t data)
{
volatile uint32_t i;
volatile uint32_t  uart_txdata;
  
for(i=0;i<8;i++) //send all 4 byte
  {
  uart_txdata = (data >> (28 - (i * 4))) & 0x0F;  
  volatile uint8_t ascii = byte_to_hex_ascii(uart_txdata);
  uart_send(UART1,ascii);//will print byte wise
  delay(1000);
  }
}

//print the BRAM stored data into Serial terminal
    void print_bram(void){
	int index=0; 
    uart_sendline(UART1,"Printing 1st 32 location of BRAM content...\n\r");
//print 1st 32 data of Progmem  
    //while(bram[index]!=0x00000039){
    while(index<32){ //print 32 words for test and debug
        print_hex(bram[index]);
       // delay(10);
        uart_sendline(UART1,"\n\r");
        index=index+1;
    }

    digitalWritePort(8,0x0);
    ss_show_word(SS_WORD_DONE);
}
//led setup
void led_setup(void)
{
    for (uint8_t pin = 8; pin <= 15; pin++)
        pinMode(pin, OUTPUT);
}

// Driver program to test above function

int main(void)
{
    uart_init(UART1);//initialize uart1
    led_setup();
    uint32_t addr = 0;
    char hex_buf[8];
    int idx = 0;
  digitalWritePort(8,0xFF);
  ss_show_word(SS_WORD_BOOT);
  delay(1000000);
 uart_sendline(UART1,"\n***KRV-32 UART Bootloader Ready***\n\r");
 uart_sendline(UART1,"\nBootloader V2.0: Copyright: S.K.Maity, School of Electronics Engg., KIIT\n\r");

  while(1){
    
    char c = uart_receive(UART1);

        if (c == '\n' || c == '\r') {
            if (idx == 8) {
                uint32_t word = hex_to_u32(hex_buf);
                /* END MARKER */
                if (word == 0xFFFFFFFF) {
                    uart_sendline(UART1,"Transfer DONE, Disable boot mode and reset the board...\n\r");
                    print_bram();
                    //while (1);        // stop here
                }
                bram[addr] = word;
                addr += 1;
            }
            idx = 0;
        } 
        else {
            if (idx < 8)
                hex_buf[idx++] = c;
        }
    }
  
    

	return 0;
}