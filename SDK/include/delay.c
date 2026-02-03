#include "delay.h"

void delay(uint32_t cycles) {
    volatile uint32_t count = 0; 
    while (count < cycles) {
        count++;
    }
}