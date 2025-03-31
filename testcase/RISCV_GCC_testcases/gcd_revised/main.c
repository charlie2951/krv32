// C program to find GCD of two numbers
// Recursive function to return gcd of a and b

#include <stdint.h>
#include <stdlib.h>

#define LEDS_START_ADDR 0x10000000   // starting addr of LED
#define LEDS_DATA_REG_OFFSET 0
#define LEDS_DATA_REG *((volatile unsigned int *)(LEDS_START_ADDR + LEDS_DATA_REG_OFFSET))


int gcd(int a, int b)
{
    // Everything divides 0
    if (a == 0)
        return b;
    if (b == 0)
        return a;

    // base case
    if (a == b)
        return a;

    // a is greater
    if (a > b)
        return gcd(a - b, b);
    return gcd(a, b - a);
}

// Driver program to test above function
int main(void)
{
	LEDS_DATA_REG = 0b000000;    
	int a = 98, b = 56;
   	LEDS_DATA_REG = gcd(a,b);
    
	return 0;
}