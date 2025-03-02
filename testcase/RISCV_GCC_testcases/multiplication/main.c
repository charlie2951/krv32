#include <stdint.h>
#include <stdlib.h>

#define LEDS_START_ADDR 1000   //256-60, 60 is starting addr of LED
#define LEDS_DATA_REG_OFFSET 0
#define LEDS_DATA_REG *((volatile unsigned int *)(LEDS_START_ADDR + LEDS_DATA_REG_OFFSET))

int mult(int a, int b)
{
	int res = a * b;
	return res;
}

int main(void)
{


    LEDS_DATA_REG = 0b000000;
    
		for(int i=0;i<10;i++) {
			for(int j=0; j<10;j++) {
				}	
					}
    
		LEDS_DATA_REG = mult(2,3);
			

    return 0;
}
