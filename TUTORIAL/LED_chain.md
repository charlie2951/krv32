### This tutorial shows how to access on board LEDs of Nexys-4 DDR board and display a LED pattern
GPIO-8 to GPIO-15 is connected to the eight on-board Green LEDs of board.
Sample code:
```
//Arduino style coding
#include "gpio.h" //to enable gpio access
#include "delay.h" //for delay function


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
```
In setup(), the LED GPIO are configured as output using `pinMode(gpio_num,OUTPUT)`. Then in loop, the data is shifted to display a shiifting patter 
of LEDs. A software delay(available inside delay.h) used to provide the intermediate delay (approx 3sec). The API `digitalWritePort(..) write the entire byte
into LED starting from 1st LED bit which is GPIO8.<p>
Save the code as `led_pattern.c` inside SDK directory. Compile and upload the firmware in board and see the moving pattern. <p>
```
> make APP=led_pattern
> python3 boot.py COM5 firmware.hex (must enable the boot mode by making J15 SW High and then press CPU_RESET - C12)
```
Reset the board after uploading done.
