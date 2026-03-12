#include "seven_seg.h"

/**
 * @brief Displays a 16-bit hex value (4 digits) on the 7-segment display.
 */
void ss_write_hex(uint16_t value) {
    SEVEN_SEG_REG = SS_MODE_HEX | (uint32_t)value;
}

/**
 * @brief Displays a hex value and controls the decimal point.
 */
void ss_write_hex_dp(uint16_t value, uint8_t dp_on) {
    uint32_t reg_val = SS_MODE_HEX | (uint32_t)value;
    if (dp_on) {
        reg_val |= SS_DP_BIT;
    }
    SEVEN_SEG_REG = reg_val;
}

/**
 * @brief Displays pre-defined words like 'DONE' or 'BOOT'.
 */
void ss_show_word(ss_word_t word) {
    SEVEN_SEG_REG = SS_MODE_WORD | (uint32_t)word;
}

/**
 * @brief Clears the display (writes 0 in hex mode).
 */
void ss_clear(void) {
    SEVEN_SEG_REG = SS_MODE_HEX | 0x0000;
}