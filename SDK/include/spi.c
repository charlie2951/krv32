#include "spi.h"

/**
 * @brief Initializes a specific SPI peripheral.
 * @param spi: Pointer to the SPI instance (SPI0 or SYS_SPI)
 * @param clk_div: Divider value (f_sclk = f_clk / (2 * (clk_div + 1)))
 * @param mode: SPI Mode (0, 1, 2, or 3)
 */
void spi_init(SPI_TypeDef* spi, uint16_t clk_div, uint8_t mode) {
    uint32_t config = (uint32_t)(clk_div & 0xFFFF);

    // Map SPI mode to CPOL/CPHA bits
    switch(mode) {
        case 1: config |= SPI_CFG_CPHA; break;
        case 2: config |= SPI_CFG_CPOL; break;
        case 3: config |= (SPI_CFG_CPOL | SPI_CFG_CPHA); break;
        case 0: 
        default: break; 
    }

    spi->CONFIG = config;        // Set divider and mode
    
    // Initial State: Start low, CS high (de-asserted)
    spi->CTRL = SPI_CTRL_CS_N; 
    
    // Wait for any power-on residual busy state to clear
    while (spi->CTRL & SPI_CTRL_BUSY);
}

/**
 * @brief Performs a full-duplex SPI byte transfer.
 * @param spi: Pointer to the SPI instance
 * @param out_byte: Byte to send
 * @return Byte received
 */
uint8_t spi_transfer(SPI_TypeDef* spi, uint8_t out_byte) {
    // 1. Load data into the data register
    spi->DATA = (uint32_t)out_byte;

    // 2. Assert START bit
    // We use bitwise OR to preserve the current state of the CS_N bit
    spi->CTRL |= SPI_CTRL_START;

    // 3. Wait for the hardware to acknowledge and finish
    // First, wait for BUSY to go HIGH (Master has started)
    while (!(spi->CTRL & SPI_CTRL_BUSY));
    
    // Then, wait for BUSY to go LOW (Transaction complete)
    while (spi->CTRL & SPI_CTRL_BUSY);

    // 4. De-assert START
    // This allows the FPGA FSM to return to IDLE from the DONE state
    spi->CTRL &= ~SPI_CTRL_START;

    // 5. Return the sampled byte
    return (uint8_t)(spi->DATA & 0xFF);
}