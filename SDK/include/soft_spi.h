#ifndef SOFT_SPI_H
#define SOFT_SPI_H

#include <stdint.h>

// GPIO Memory Map
#define GPIO_BASE    0x10000000 
#define GPIO_DATA    (*(volatile uint32_t*)(GPIO_BASE + 0x00))
#define GPIO_DIR     (*(volatile uint32_t*)(GPIO_BASE + 0x04))

// Pin Definitions
#define PIN_SCLK (1 << 0)
#define PIN_MOSI (1 << 1)
#define PIN_MISO (1 << 2)
#define PIN_CS   (1 << 3)

// Function Prototypes
void spi_bb_init(void);
uint8_t spi_bb_transfer(uint8_t out_byte);
void spi_bb_cs_low(void);
void spi_bb_cs_high(void);

#endif