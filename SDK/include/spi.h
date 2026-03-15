#ifndef SPI_H
#define SPI_H

#include <stdint.h>

// Base Addresses
#define SPI_REG_CTRL    (*((volatile uint32_t*)0xC1000000))
#define SPI_REG_DATA    (*((volatile uint32_t*)0xC1000004))

// Control Bits
#define SPI_START       (1 << 0)
#define SPI_BUSY        (1 << 1)
#define SPI_CS_BIT      (1 << 2)

// Function Prototypes
void spi_init(void);
uint8_t spi_transfer(uint8_t out_byte);
void spi_cs_low(void);
void spi_cs_high(void);

#endif