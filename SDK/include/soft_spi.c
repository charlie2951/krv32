#include "soft_spi.h"

void spi_bb_init() {
    GPIO_DIR |= (PIN_SCLK | PIN_MOSI | PIN_CS);
    GPIO_DIR &= ~(PIN_MISO);
    GPIO_DATA |= PIN_CS;   // CS Idle High
    GPIO_DATA &= ~PIN_SCLK; // SCLK Idle low
}

static inline void spi_delay() {
    for (volatile int i = 0; i < 100; i++); 
}

uint8_t spi_bb_transfer(uint8_t out_byte) {
    uint8_t in_byte = 0;
    for (int i = 7; i >= 0; i--) {
        if (out_byte & (1 << i)) GPIO_DATA |= PIN_MOSI;
        else                     GPIO_DATA &= ~PIN_MOSI;

        spi_delay();
        GPIO_DATA |= PIN_SCLK; // Rising edge
        spi_delay();

        if (GPIO_DATA & PIN_MISO) in_byte |= (1 << i);

        GPIO_DATA &= ~PIN_SCLK; // Falling edge
    }
    return in_byte;
}

void spi_bb_cs_low()  { GPIO_DATA &= ~PIN_CS; }
void spi_bb_cs_high() { GPIO_DATA |= PIN_CS;  }