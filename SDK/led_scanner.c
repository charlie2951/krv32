#include "led_driver.h"
#include "delay.h"

void test_led_logic() {
    // 1. Clear all LEDs initially
    led_write(0x00000000);
    delay(100000);

    // 2. Sequence: Turn them on one by one
    for (int i = 0; i < 8; i++) {
        led_set_bit(i);     // Turn on LED i
        delay(500000);      // Wait so Subeer can see it
    }

    // 3. Sequence: Turn them off one by one in reverse
    for (int i = 7; i >= 0; i--) {
        led_clear_bit(i);   // Turn off LED i
        delay(500000);
    }
}

int main() {
    while(1) {
        test_led_logic();
    }
    return 0;
}