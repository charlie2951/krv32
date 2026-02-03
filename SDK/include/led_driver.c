#include "led_driver.h"

void led_write(uint32_t value) {
    LED_REG = value;
}

void led_set_bit(uint8_t bit_num) {
    LED_REG |= (1 << bit_num);
}

void led_clear_bit(uint8_t bit_num) {
    LED_REG &= ~(1 << bit_num);
}