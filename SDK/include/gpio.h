#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/* GPIO register map */
#define GPIO_DATA ((volatile uint32_t *)0x10000000)
#define GPIO_DIR  ((volatile uint32_t *)0x10000004)

/* Arduino-style modes */
#define INPUT   0
#define OUTPUT  1

/* Arduino-style values */
#define LOW     0
#define HIGH    1

/* GPIO API */
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
uint8_t digitalRead(uint8_t pin);

/* New APIs */
void pinToggle(uint8_t pin);
void digitalWritePort(uint8_t base_pin, uint8_t value);

#endif /* GPIO_H */
