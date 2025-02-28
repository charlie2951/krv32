#include <stdint.h>
#include <stdlib.h>

#define LEDS_START_ADDR 2000
#define LEDS_DATA_REG_OFFSET 0
#define LEDS_DATA_REG *((volatile unsigned int *)(LEDS_START_ADDR + LEDS_DATA_REG_OFFSET))

int main(void)
{

    LEDS_DATA_REG = 0b000000;
    int  i=0, j=0, k=0;

    while (1)
    {
	if(LEDS_DATA_REG == 64)
	LEDS_DATA_REG = 0;
       
       for(i=0;i<1000;i++)
	{
		for(j=0;j<1000;j++){
		}
	}
     	LEDS_DATA_REG = LEDS_DATA_REG + 1;		
}
    return 0;
}
