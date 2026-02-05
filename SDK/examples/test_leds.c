//Arduino style coding
#include "gpio.h"
#include "delay.h"


void setup(void)
{
    for (uint8_t pin = 8; pin <= 15; pin++)
        pinMode(pin, OUTPUT);
}

int main(void)
{
     setup();
     while(1){
    digitalWritePort(8, 0xFF);   // fast 8-bit LED write
    delay(500000);
    digitalWritePort(8, 0x00);   // fast 8-bit LED write
    delay(500000);
     }
}
