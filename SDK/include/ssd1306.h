#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include "i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SSD1306_ADDR  0x3C

// Initialize and select I2C instance
void ssd1306_init(i2c_t *i2c);

// Display functions (no i2c argument required)
void ssd1306_clear(void);
void ssd1306_set_cursor(uint8_t page, uint8_t col);
void ssd1306_write_char(char c);
void ssd1306_write_string(const char *str);

#ifdef __cplusplus
}
#endif

#endif
