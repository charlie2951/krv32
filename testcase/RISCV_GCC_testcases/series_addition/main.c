// *** C program to add numbers starting from 1 to 10 and displays on LED port *** //

#include <stdint.h>
#include <stdlib.h>

#define LEDS_START_ADDR 2000
#define LEDS_DATA_REG_OFFSET 0
#define LEDS_DATA_REG *((volatile unsigned int *)(LEDS_START_ADDR + LEDS_DATA_REG_OFFSET))

int main(void)
{

    LEDS_DATA_REG = 0b000000;
    int  i=1, j=0;

    
       for(i=1; i<11; i++)
         j = j + i;
	
     	LEDS_DATA_REG = j;		

    return 0;
}
