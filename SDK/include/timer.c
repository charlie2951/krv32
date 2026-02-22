#include "timer_revised.h"

void timer_set_value(Timer_TypeDef *timer, uint32_t value) {
    timer->TIMER_VAL = value;
}

void timer_init(Timer_TypeDef *timer, uint32_t mode) {
    timer->CTRL = (mode & 0x01) << 1; 
}

void timer_start(Timer_TypeDef *timer) {
    timer->CTRL |= (1u << 0);  // Start
    timer->CTRL &= ~(1u << 0); // Stop (Pulse)
}

uint32_t timer_status(Timer_TypeDef *timer) {
    if (timer->STATUS & 0x01) {
        timer->STATUS = 0; // Write 0 to clear
        return 1;
    }
    return 0;
}

void timer_delay_us(Timer_TypeDef *timer, uint32_t val) {
    uint32_t counts = (val << 6) + (val << 5) + (val << 2);
    timer_set_value(timer, counts);
    timer_init(timer, 0);
    timer_start(timer);
    while(timer_status(timer) == 0); 
}

void timer_delay_ms(Timer_TypeDef *timer,uint32_t ms) 
{
    while (ms--) 
    {
        // 1000 microseconds = 1 millisecond
        timer_delay_us(timer,1000); 
    }
}
