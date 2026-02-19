/*
MIT License

Copyright (c) 2024-2026 Subir Maity

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
/*
   Description: GPIO access driver
                total 16 GPIO (0 to 7 via PMOD JB and 8-15 mapped to LEDs)
   Author: Subir Maity
   Version: 1.0
*/
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

// ---------- New functions ---------- //

void pinToggle(uint8_t pin)//not working -bug
{
    *GPIO_DATA ^= (0x00000001 << pin);
}


 // Write 8-bit value to a GPIO port starting at base_pin
 //Example:
 //  digitalWritePort(8, 0xAA);  // GPIO8–GPIO15
 //
void digitalWritePort(uint8_t base_pin, uint8_t value)
{
    uint32_t mask = (0x000000FF << base_pin);

    *GPIO_DATA &= ~mask;                     // clear port
    *GPIO_DATA |= ((uint32_t)value << base_pin);
}



