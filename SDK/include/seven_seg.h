#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

#include <stdint.h>

/* Base Address defined in Verilog IP */
#define SEVEN_SEG_BASE    0x11000000
#define SEVEN_SEG_REG     (*(volatile uint32_t*)SEVEN_SEG_BASE)

/* Bit-field Definitions */
#define SS_MODE_HEX       (0 << 17)
#define SS_MODE_WORD      (1 << 17)
#define SS_DP_BIT         (1 << 16)

/* Special Word Codes (matching Verilog case statements) */
typedef enum {
    SS_WORD_NONE = 0,
    SS_WORD_DONE = 1,
    SS_WORD_BOOT = 2,
    SS_WORD_FAIL = 3
} ss_word_t;

/* Function Prototypes */
void ss_write_hex(uint16_t value);
void ss_write_hex_dp(uint16_t value, uint8_t dp_on);
void ss_show_word(ss_word_t word);
void ss_clear(void);

#endif // SEVEN_SEG_H