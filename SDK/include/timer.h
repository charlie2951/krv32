#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

typedef struct {
    volatile uint32_t CTRL;      // Offset 0x00
    volatile uint32_t TIMER_VAL; // Offset 0x04
    volatile uint32_t COUNT_VAL; // Offset 0x08
    volatile uint32_t STATUS;    // Offset 0x0C
} Timer_TypeDef;

// Access pointers
#define TIMER0 ((Timer_TypeDef *)0xB0000000UL)
#define TIMER1 ((Timer_TypeDef *)0xB0000010UL)

// API Prototypes updated to accept a pointer to the specific timer
void timer_init(Timer_TypeDef *timer, uint32_t mode);
void timer_set_value(Timer_TypeDef *timer, uint32_t value);
void timer_start(Timer_TypeDef *timer);
uint32_t timer_status(Timer_TypeDef *timer);
void timer_delay_us(Timer_TypeDef *timer, uint32_t val);
void timer_delay_ms(Timer_TypeDef *timer, uint32_t val);
void timer_getcount(Timer_TypeDef *timer,uint32_t *count);
#endif
