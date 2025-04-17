// C program to implement a delay
// Example uses: A binary counter

#include <stdint.h>
#include <stdlib.h>

#define LEDS_START_ADDR 0x10000000   // starting addr of LED
#define LEDS_DATA_REG *((volatile unsigned int *)(LEDS_START_ADDR ))

void delay(uint32_t cycles) {
  volatile uint32_t count = 0; // volatile to prevent compiler optimization

  while (count < cycles) {
    count++;
  }
}


// Driver program to test above function
int main(void)
{
LEDS_DATA_REG = 0b000000;
while(1){
	    
	delay(100000);// total delay coming 1 sec approx
//delay(5);//for debugging purpose only
	
   	LEDS_DATA_REG = LEDS_DATA_REG + 1;
}
    
	return 0;
}