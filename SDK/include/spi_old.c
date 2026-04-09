#include "spi.h"

void spi_init(void) {
    SPI_REG_CTRL &= ~SPI_START; // Ensure FSM is IDLE
    SPI_REG_CTRL |= SPI_CS_BIT;  // Ensure CS is high (de-asserted)
    while (SPI_REG_CTRL & SPI_BUSY);
}

void spi_cs_low(void) {
    SPI_REG_CTRL &= ~SPI_CS_BIT;
}

void spi_cs_high(void) {
    SPI_REG_CTRL |= SPI_CS_BIT;
}

uint8_t spi_transfer(uint8_t out_byte) {
    SPI_REG_CTRL &= ~SPI_START;
    SPI_REG_DATA = (uint32_t)out_byte;
    SPI_REG_CTRL |= SPI_START;
    
    // Handshake delays based on CLK_DIV 50
    for(volatile int i = 0; i < 20; i++); 
    for(volatile int i = 0; i < 2500; i++); 

    uint8_t in_byte = (uint8_t)(SPI_REG_DATA & 0xFF);
    SPI_REG_CTRL &= ~SPI_START;
    
    while (SPI_REG_CTRL & SPI_BUSY);
    return in_byte;
}