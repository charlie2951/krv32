/*
KRV-32 Bootloader with Flash support
Version-3.0
Copyright: Subir Maity
Supported Device: 
-Basys-3 board with Macronix flash
*/
#include <stdint.h>
//#include "delay.h"
//#include "spi.h"
//#include "mx25.h"
#include "seven_seg.h"
#include "printf.h"
#include "uart.h"
#include "gpio.h"

//BRAM mem location//
#define BRAM_BASE   0x00000000
volatile uint32_t *bram = (uint32_t *)BRAM_BASE;

//helper function hex_to_u32()
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

//print the BRAM stored data into Serial terminal
    void print_bram(void){
	int index=0; 
    printf("Printing BRAM content...\n\r");
//print BRAM content  
    while(bram[index]!=0xABCDEF99) //END OF BRAM DATA
    {
        printf("\r\nIndex: %d, Data: %x",index, bram[index]);
        index=index+1;
    }
}

// Main function //
int main(void){
    uint32_t addr = 0;
    char hex_buf[8];
    int idx = 0;
    //configure GPIOs
    for (uint8_t pin = 8; pin <= 15; pin++)
        pinMode(pin, OUTPUT); //configure GPIO8-15 (LEDS) as output
    ss_show_word(SS_WORD_BOOT); //display BOOT message 
    digitalWritePort(8,0xFF);//all LEDs are ON
    printf("\n***KRV32 UART Bootloader Ready***\n\r");
    printf("\nBootloader V3.0: Copyright: S.K.Maity, School of Electronics Engg., KIIT\n\r");
    
    while(1)
    {
    char c = (char)uart_receive(UART1);

        if (c == '\n' || c == '\r') {
            if (idx == 8) {
                uint32_t word = hex_to_u32(hex_buf);
                /* END MARKER */
                if (word == 0xFFFFFFFF) {
                   // LED_DATA = 0x1;   // LED ON
                    printf("Transfer DONE, Disable boot mode and reset the board...\n\r");
                   // print_bram();//print data on serial monitor
                    ss_show_word(SS_WORD_DONE);
                    digitalWritePort(8,0x00);//all LEDs are OFF
                    while (1);        // stop here
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