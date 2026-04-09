#ifndef SPI_H
#define SPI_H

#include <stdint.h>

// --- Base Addresses ---
#define SPI0_BASE    0xC1000000UL   // Standard SPI (e.g., Pmods)
#define SYS_SPI_BASE 0xC2000000UL   // System Flash (Macronix)

// --- Register Structure ---
typedef struct {
    volatile uint32_t CTRL;   // Offset 0x00: Start[0], Busy[1], CS[2]
    volatile uint32_t DATA;   // Offset 0x04: TX/RX Byte
    volatile uint32_t CONFIG; // Offset 0x08: Div[15:0], CPOL[16], CPHA[17]
} SPI_TypeDef;

// --- Interface Handles ---
#define SPI0    ((SPI_TypeDef*)SPI0_BASE)
#define SYS_SPI ((SPI_TypeDef*)SYS_SPI_BASE)

// --- Bit Definitions ---
#define SPI_CTRL_START  (1u << 0)
#define SPI_CTRL_BUSY   (1u << 1)
#define SPI_CTRL_CS_N   (1u << 2) // 1 = High (De-selected), 0 = Low (Selected)

#define SPI_CFG_CPOL    (1u << 16)
#define SPI_CFG_CPHA    (1u << 17)

// --- Function Prototypes ---

/**
 * Initializes the SPI port with a specific divider and mode.
 * Mode 0: CPOL=0, CPHA=0 | Mode 3: CPOL=1, CPHA=1
 */
void spi_init(SPI_TypeDef* spi, uint16_t clk_div, uint8_t mode);

/**
 * Transmits 1 byte and returns the received byte.
 * Automatically handles waiting for the BUSY bit.
 */
uint8_t spi_transfer(SPI_TypeDef* spi, uint8_t out_byte);

/**
 * Manual Chip Select Control
 */
static inline void spi_cs_low(SPI_TypeDef* spi)  { spi->CTRL &= ~SPI_CTRL_CS_N; }
static inline void spi_cs_high(SPI_TypeDef* spi) { spi->CTRL |= SPI_CTRL_CS_N;  }

#endif