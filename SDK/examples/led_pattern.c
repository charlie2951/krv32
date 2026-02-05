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
    uint8_t pat = 1;
     setup();
     while(1){
    digitalWritePort(8,pat);
    pat = (pat << 1) | (pat >> 7);
    delay(300000);
     }
}