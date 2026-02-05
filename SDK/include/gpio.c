#include <stdint.h>
#include "gpio.h"

void pinMode(uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT)
        *GPIO_DIR |= (0x00000001 << pin);
    else
        *GPIO_DIR &= ~(0x00000001 << pin);
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    if (value == HIGH)
        *GPIO_DATA |= (0x00000001 << pin);
    else
        *GPIO_DATA &= ~(0x00000001 << pin);
}

uint8_t digitalRead(uint8_t pin)
{
    return (uint8_t)((*GPIO_DATA >> pin) & 0x1);
}

/* ---------- New functions ---------- */

void pinToggle(uint8_t pin)//not working -bug
{
    *GPIO_DATA ^= (0x00000001 << pin);
}

/*
 * Write 8-bit value to a GPIO port starting at base_pin
 * Example:
 *   digitalWritePort(8, 0xAA);  // GPIO8–GPIO15
 */
void digitalWritePort(uint8_t base_pin, uint8_t value)
{
    uint32_t mask = (0x000000FF << base_pin);

    *GPIO_DATA &= ~mask;                     // clear port
    *GPIO_DATA |= ((uint32_t)value << base_pin);
}
