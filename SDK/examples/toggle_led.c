//Arduino style coding
#include "gpio.h"
#include "delay.h"


void setup(void)
{
    //for (uint8_t pin = 8; pin <= 15; pin++)
       // pinMode(pin, OUTPUT);
       pinMode(10, OUTPUT);
}

int main(void)
{
     setup();
     while(1){
    digitalWrite(10,1);
    delay(2000000);
    digitalWrite(10,0);
    delay(2000000);

     }
}
