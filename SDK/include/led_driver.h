#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>

// Hardware Address Definition
#define LED_ADDR      0x10000000
#define LED_REG       (*((volatile uint32_t *)(LED_ADDR)))

/**
 * @brief Sets the LED output to a specific 8-bit or 32-bit value.
 * @param value The pattern to display on the LEDs.
 */
void led_write(uint32_t value);

/**
 * @brief Sets a specific bit of the LED register.
 * @param bit_num The index of the LED (0-7 usually).
 */
void led_set_bit(uint8_t bit_num);

/**
 * @brief Clears a specific bit of the LED register.
 * @param bit_num The index of the LED to turn off.
 */
void led_clear_bit(uint8_t bit_num);

#endif